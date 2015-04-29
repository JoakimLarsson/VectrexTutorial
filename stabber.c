/* Vectrex Tutorial stabber tool by Joakim Larsson Edström, (c) 2015, GPLv3 */

/*
 * This is a tool to intermix assembler with source code based on the
 * STABS debugformat from the AS6809 .LST, .RST and .S files generated
 * with the -g flag to GCC and -y flag to as6809. Other compilers and
 * assemblers may also be able to produce STABS. The STABS format is 
 * used in COFF output files too but in binary form I think and not 
 * not tested. 
 * 
 * In general STABS is nowadays replaced by the DWARF debug format and
 * others but most 8 bit tools are too old to generate DWARF.  
 *
 * Read more about STABS here: https://docs.freebsd.org/info/stabs/stabs.pdf
 * 
 * Compile with:
 *
 *   gcc -o stabber stabber.c ; 
 *
 * Usage:
 *
 * cat bouncer5.lst | ./stabber
*/

#include <stdio.h>
#include <string.h>

main()
{
  FILE *fp = NULL;
  char buf[200];
//  int current_line;
  char *p;
  char desc[200], data[200], value[200];
  int  type, other, dq;

  printf("; Mix by the STAB decoder\n");
  while (fgets(buf, sizeof(buf), stdin) != NULL)
  {
    if ((p = strstr(buf, ".stab")) != NULL)
    {
      char *p1;
      char last = ' ';
      int i, j;
      p1 = &p[6];
      dq = 0;

      for (i = 6; i < 200 && p[i] != '\r' && p[i] != '\n'; i++)
      {
	switch (p[i])
	{
	  case '"': 
	    dq = dq == 0 ? 1 : 0;
	    *p1 = ' ';
	    p1 += last != ' ' ? 1 : 0;
	    last = ' ';
	    break;
	  case ' ':
	    if (dq != 0) // inside double quotes we mark spaces with + to enable sscanf to treat it as one string
	    {
	      *p1 = '+';
	      p1 += 1;
	      break;
	    }
	    *p1 = ' ';
	    p1 += last != ' ' ? 1 : 0;
	    last = ' ';
	    break;
	  case ',':
	    if (dq != 0) // inside double quotes we mark spaces with + to enable sscanf to treat it as one string
	    {
	      *p1 = '|';
	      p1 += 1;
	      break;
	    }
	    *p1 = ' ';
	    p1 += last != ' ' ? 1 : 0;
	    last = ' ';
	    break;
	  default:
	    *p1++ = p[i];
	    last = p[i];
	    break;
	}
      }
      *p1 = '\0';

      if (p[5] == 's')
      {
	char types[200];

	sscanf(&p[6], "%s %d %d %s %s", data, &type, &other, desc, value);
//	printf("S: |%s| 0x%x %d |%s| |%s|\n", data, type, other, desc, value);
	switch(type)
	{
	case 0x20:
	  strncpy(types,"N_GSYM", sizeof(types)); 
	  printf("; %s Global symbol called %s\n", types, data); 
	  break;
	case 0x24:
	  strncpy(types,"N_FUN", sizeof(types)); 
	  printf("; %s function called %s\n", types, data); 
	  break;
	case 0x26:
	  strncpy(types,"N_STSYM", sizeof(types)); 
	  printf("; %s Data section file-scope static initialized variable %s\n", types, data); 
	  break;
	case 0x28:
	  strncpy(types,"N_LCSYM", sizeof(types)); 
	  printf("; %s BSS section file-scope static un-initialized variable %s\n", types, data); 
	  break;
	case 0x3c:
	  strncpy(types,"N_OPT", sizeof(types)); 
	  printf("; The STAB information was generated because the source was %s\n", data); 
	  break;
	case 0x40:
	  strncpy(types,"N_RSYM", sizeof(types)); 
	  printf("; %s Register variable %s\n", types, data); 
	  break;
	case 0x64:
	  strncpy(types,"N_SO", sizeof(types)); 
	  printf("; %s source file %s\n", types, data); 
	  fp = fopen(data, "r");
//	  current_line = 0;
	  break;
	case 0xa0:
	  strncpy(types,"N_PSYM", sizeof(types)); 
	  printf("; %s stack parameter %s at stack offset %s\n", types, data, value); 
	  break;
	case 0x80: /* Symbol definitions */
	  {
	    char name[200], exp[200], *p2;

	    strncpy(types,"N_LSYM", sizeof(types));
	    
#if 0 /* This was much more complex than I thought, needs some more tinkering... */
	    p1 = data;
	    do { 
	      p2 = strchr(p1, ';');
	      if ( p2 != NULL) *p2++ = '\0';
	      while (*p1) { *p1 = (*p1 == ':' ? ' ' : *p1); p1++; }
	      sscanf(data, "%s %s", name, exp);
	      p1 = name; while (*p1) { *p1 = (*p1 == '+' ? ' ' : *p1); p1++; }
	      p1 = exp; while (*p1) { *p1 == '|' ? ',' : *p1; p1++; }
	      printf("; Local Symbol %s: %s\n", name, exp);
	      p1 = p2;
	    } while (p2 != NULL); 
#else
	      p1 = data; while (*p1) { *p1 = (*p1 == ':' ? ' ' : *p1); p1++; }
	      sscanf(data, "%s %s", name, exp);
	      printf("; Local symbol %s: %s\n", name, exp);
#endif
	  }
	  break;
	default:
	  snprintf(types, sizeof(types), "0x%x", type);
	  printf("; Unknown S record type |%s| %s %d |%s| |%s|\n", data, types, other, desc, value);
	}
      }
      else if (p[5] == 'd')
      {
	char types[200];

	sscanf(&p[6], "%d %d %s", &type, &other, desc);
	switch(type)
	{
	case 0x44:
	  strncpy(types,"N_SLINE", sizeof(types)); 
	  if (fp != NULL)
	  {
	    char lbf[200];
#if 0 // This doesn't work because the compiler moves around code while optimizing
	    signed long line = strtol(desc, NULL , 10) - current_line;
	    current_line += line;
	    while (line-- > 0)
	    { 
	      fgets(lbf, sizeof(buf), fp);
	      printf("%s", lbf);
	    }
#else
	    signed long line = strtol(desc, NULL , 10);
	    rewind(fp);
	    while (line-- > 0)
	    { 
	      fgets(lbf, sizeof(buf), fp);
	    }
	    printf("; %s", lbf);
#endif
	  }
	  break;
	default:
	  snprintf(types, sizeof(types), "0x%x", type);
	  printf("; Unknown D record type %s %d |%s|\n", types, other, desc);
	}
      }
      else
	printf("; ?: Unknown record type: %s\n", p);
    }
    else
      /* Just remove pagination headers and footers */
      if (buf[0] == ' ' || buf[0] == '\t')
	printf("%s", buf);
  }
  if (fp != NULL)
    fclose(fp);
}
