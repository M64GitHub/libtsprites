// tsscener.hpp - libtsprites, 2023-24 M64

#ifndef TSL_SCENER_H
#define TSL_SCENER_H

#include "tsprites.hpp"



typedef struct s_scener_sprite_register_entry {
  char name[128];
  void *sprite;

} TSScRegEntry;

class TSScener
{
public:
  TSScener();
  ~TSScener();

  void Init();
  void RegisterSprite(void *sprite, char *name);

private:
  TSScRegEntry *sprite_register;
};



#endif

