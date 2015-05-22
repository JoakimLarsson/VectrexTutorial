

#define BSIZE 127
#define VEXPRITES 5
#define SCALE 200
#define SPEED 1



/*
 * vectorlist class
 * 
 * A class to keep track of the boundaries of a vector symbol. The constructor calculates the boundaries
 * when created by the 'new' operator and passed a Vectrex vectorlist. It only adds a RAM usage of 6 bytes 
 * per unique vectorlist, NOT per animated object! 
 */
class vectorlist {
public:
  vectorlist(int8_t *sym);
  int8_t *vec;      /* Vectrex vector list */
  int8_t  bleft;    /* Left boundary  */
  int8_t  bright;   /* Right boundary */
  int8_t  bup;      /* Up boundary    */
  int8_t  bdown;    /* Down boundary  */
private:
  void init(int8_t *sym);
};


/* 
 * Vexprites class
 *
 * A class to keep track of the position, direction and speed of an object. Each object requires 
 * only 6 bytes of RAM.
 */
class vexprite 
{
public:
  vectorlist   *sym;           /* vectorlist */
  int8_t        ox, oy;        /* Centerpoint of vexprite    */
  int8_t        xdir, ydir;    /* Speed and direction of vexprite */
#if THREED
  int8_t        oz;            /* Centerpoint in depth */
  int8_t        zdir;          /* Speed and z component of direction */
#endif
  vexprite();
  void draw();
  void move();
};

/* 
 * The following vectorlists ends up readonly in flash memory and take NO RAM space 
 */

/* 
 * Simple square
 * Profile ROM:  11 RAM:   0 Bouncer5.box_line_list: On init 
 */
#define BX_SIZE 3

int8_t box_line_list[] = 
  { 4,
     BX_SIZE / 2, BX_SIZE / 2,
    -BX_SIZE,     0,
     0,       -BX_SIZE,
     BX_SIZE,     0,
     0,        BX_SIZE    
  };

/* Turtle outline ripped from the line2 tutorial 
 * Profile ROM:  47 RAM:   0 Bouncer5.turtle_line_list: On init 
 */
#define TU_SIZE 1

int8_t turtle_line_list[] = 
{   23,
     2 * TU_SIZE,  2 * TU_SIZE,
    -1 * TU_SIZE,  2 * TU_SIZE,
     2 * TU_SIZE,  1 * TU_SIZE,
     2 * TU_SIZE, -2 * TU_SIZE,
     0 * TU_SIZE,  2 * TU_SIZE,
    -1 * TU_SIZE,  1 * TU_SIZE,
     1 * TU_SIZE,  3 * TU_SIZE,
    -1 * TU_SIZE,  4 * TU_SIZE,
     1 * TU_SIZE,  0 * TU_SIZE,
    -1 * TU_SIZE,  1 * TU_SIZE,
    -1 * TU_SIZE,  0 * TU_SIZE,
    -3 * TU_SIZE,  2 * TU_SIZE,
    -3 * TU_SIZE, -2 * TU_SIZE,
    -1 * TU_SIZE,  0 * TU_SIZE,
    -1 * TU_SIZE, -1 * TU_SIZE,
     1 * TU_SIZE,  0 * TU_SIZE,
    -1 * TU_SIZE, -4 * TU_SIZE,
     1 * TU_SIZE, -3 * TU_SIZE,
    -1 * TU_SIZE, -1 * TU_SIZE,
     0 * TU_SIZE, -2 * TU_SIZE,
     2 * TU_SIZE,  2 * TU_SIZE,
     2 * TU_SIZE, -1 * TU_SIZE,
    -1 * TU_SIZE, -2 * TU_SIZE,
     2 * TU_SIZE, -2 * TU_SIZE 
};

