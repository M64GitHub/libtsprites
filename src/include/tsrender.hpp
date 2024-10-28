// tsrender.hpp - libtsprites, 2023-24, M64

#ifndef TSL_RENDER_H
#define TSL_RENDER_H

#include "tsrendersurface.hpp"

class TSRenderEngineTopDown {
public:
  RenderSurface_t *Render(RenderSurface_t *surfaces_in[], int n,
                         RenderSurface_t *surface_out);

private:
};

#endif
