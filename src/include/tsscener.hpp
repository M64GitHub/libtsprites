// tsscener.hpp - libtsprites, 2023-24 M64

#ifndef TSL_SCENER_H
#define TSL_SCENER_H

typedef struct ScenerSpriteRegistry_s {
  char name[128];
  void *sprite;

} ScenerSpriteRegistryEntry_t;

class TSScener
{
public:
  TSScener();
  ~TSScener();

  void Init();
  void RegisterSprite(void *sprite, char *name);

private:
  ScenerSpriteRegistryEntry_t *sprite_register;
};



#endif

