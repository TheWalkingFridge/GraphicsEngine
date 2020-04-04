#include <ModelTriangle.h>
#include <DrawingWindow.h>
#include <glm/glm.hpp>
#include <fstream>
#include <vector>
#include "draw/Drawer.hpp"
#include "external/OBJFile.h"
#include "model/Camera.hpp"
#include "model/Light.hpp"
#include "model/World.hpp"
#include "EventHandler.cpp"
#include "Utilities.h"
#include "draw/Drawer.hpp"

using namespace std;
using namespace glm;

void draw();
void update();

const int WIDTH = 600;
const int HEIGHT = 400;

Camera camera = Camera(0.0f, 0.0f, 3.0f, 1.0f);
Light light = Light(0.0f, 0.9f, 0.0f, 40.0f);
World world = World(camera, light);

DrawingWindow window = DrawingWindow(WIDTH, HEIGHT, false);
unordered_map<string, int> state;

bool isSpinning = false;
bool showRaytracing = true;
int mode = 0;

int main(int argc, char *argv[])
{
    state["rotateAnimation"] = 0;
    state["displayMode"] = 0;

    // world.addMesh(OBJFile("assets/CornellBox/cornell-box.obj", 1.0f));
    // world.addMesh(OBJFile("assets/HackSpaceLogo/logo.obj", 1.0f));
    world.addMesh(OBJFile("assets/Checkerboard/checkerboard.obj", 1.0f));

    EventHandler handler = EventHandler(window, world, state);

    while (true)
    {
        // We MUST poll for events - otherwise the window will freeze !
        handler.listenForEvents();
        update();
        draw();

        // Need to render the frame at the end, or nothing actually gets shown on the screen !
        window.renderFrame();
    }
}

void draw()
{
    if(state["displayMode"] == 0)
    {
        window.clearPixels();
        drawModelWireframe(world, window);
    } 
    
    else if(state["displayMode"] == 1)
    {
        window.clearPixels();
        drawModel(world, window, false);
    }
}

void update()
{
    // Function for performing animation (shifting artifacts or moving the camera)
    if (state["rotateAnimation"] == 0)
        return;

    // world.camera.translate(glm::vec3(1.0f, 0.0f, 0.0f), 0.3f);
    world.camera.rotate(glm::vec3(0.0f, 0.0f, 0.0f), 3.5f);
    world.camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));
    world.transformMeshToCameraSpace();
}
