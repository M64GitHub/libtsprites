// tsrender.hpp - libtsprites, 2023-24, M64

#ifndef TSL_RENDER_H
#define TSL_RENDER_H

#include "tscolors.hpp"
#include "tsprites.hpp"
#include "tsrendersurface.hpp"

class TSRenderEngineTopDown {
public:
  render_surface *Render(render_surface *surfaces_in[], int n,
                         render_surface *surface_out);

private:
};

#endif
