#include <Colour.h>
#include <glm/glm.hpp>

#include "Utilities.h"
#include "DrawUtils.h"


// PUBLIC
namespace rasterizer {
void draw(OBJFile model, Camera camera, DrawingWindow window, bool showWireframe) 
{
     // Create a depth buffer
    float *depthBuffer = new float[window.width * window.height];
    std::fill_n(depthBuffer, window.width * window.height, std::numeric_limits<float>::infinity());

    for (ModelTriangle modelTriangle : model.loadedFaces)
    {
        if(modelTriangle.vertices[0].z > 0) continue;
        if(modelTriangle.vertices[1].z > 0) continue;
        if(modelTriangle.vertices[2].z > 0) continue;
        
        CanvasTriangle canvasTriangle = utilities::convertToCanvasTriangle(modelTriangle, camera, window);

        drawUtilities::drawTriangle(canvasTriangle, !showWireframe, window, depthBuffer);
    }
}
}

