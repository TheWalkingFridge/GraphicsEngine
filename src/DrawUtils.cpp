#include "DrawUtils.h"
#include "Utilities.h"
#include "ModelTriangle.h"

#include <cmath>
#include <vector>

void fillTriangle(CanvasTriangle triangle, Colour c, DrawingWindow window);
void fillTriangle(CanvasTriangle triangle, PPMImage image, DrawingWindow window);
void fillTriangle(CanvasTriangle triangle, Colour c, DrawingWindow window, float* depthBuffer);

void drawRedError(DrawingWindow window){
   for(int y=0; y<window.height ;y++) {
    for(int x=0; x<window.width ;x++) {
      float red = rand() % 255;
      float green = 0.0;
      float blue = 0.0;
      uint32_t colour = (255<<24) + (int(red)<<16) + (int(green)<<8) + int(blue);
      window.setPixelColour(x, y, colour);
    }
  }
}

void drawGrayGradient(DrawingWindow window){
    std::vector<float> greyGradient = utilities::interpolate(255, 0, 320);

    for(int y=0; y<window.height ;y++) {
        for(int x=0; x<window.width ;x++) {
            float red = greyGradient[x];
            float green = greyGradient[x];
            float blue = greyGradient[x];
            uint32_t colour = (255<<24) + (int(red)<<16) + (int(green)<<8) + int(blue);
            window.setPixelColour(x, y, colour);
        }
    }
}

void drawColorGradient(DrawingWindow window){

  vec3 red   ( 255, 0,   0 );
  vec3 green ( 0, 255,   0 );
  vec3 blue  ( 0,   0, 255 );
  vec3 yel   ( 255,255,  0 );

  std::vector<vec3> leftCol = utilities::interpolate3d(red, yel, HEIGHT);
  std::vector<vec3> rightCol = utilities::interpolate3d(blue, green, HEIGHT);
  // vector<vec3> topCol = interpolate3d(blue, green, HEIGHT

   for(int y=0; y<window.height ;y++) {

    vec3 start = leftCol[y];
    vec3 end   = rightCol[y];

    std::vector<vec3> grad = utilities::interpolate3d(start, end, WIDTH);

    for(int x=0; x<window.width ;x++) {
      float red = grad[x].r;
      float green = grad[x].g;
      float blue = grad[x].b;

      uint32_t colour = (255<<24) + (int(red)<<16) + (int(green)<<8) + int(blue);
      window.setPixelColour(x, y, colour);
    }
  }
}

void drawLine(CanvasPoint a, CanvasPoint b, Colour c, DrawingWindow window)
{
  uint32_t colour = (255<<24) + (c.red<<16) + (c.green<<8) + c.blue;
  int xDiff = b.x - a.x;
  int yDiff = b.y - a.y;

  float steps = std::max(abs(xDiff), abs(yDiff));
  if(steps == 0) return;

  float xStepSize = xDiff / steps;
  float yStepSize = yDiff / steps;

  for(int step=0; step < steps; step++) {
    float x = a.x + step * xStepSize;
    float y = a.y + step * yStepSize;

    window.setPixelColour(x, y, colour);
  }
}

void drawTriangleOutline(CanvasTriangle triangle, Colour c, DrawingWindow window) 
{
    drawLine(triangle.vertices[0], triangle.vertices[1], c, window);
    drawLine(triangle.vertices[1], triangle.vertices[2], c, window);
    drawLine(triangle.vertices[2], triangle.vertices[0], c, window);
}

void drawTriangleFilled(CanvasTriangle triangle, Colour c, DrawingWindow window) 
{
  // Sort verticies
  std::sort(triangle.vertices, triangle.vertices+3, [](CanvasPoint const & a, CanvasPoint const & b) -> bool { return a.y < b.y; } );

  CanvasPoint midPoint = utilities::getTriangleMidPoint(triangle);

  fillTriangle(CanvasTriangle(triangle.vertices[0], triangle.vertices[1], midPoint), c, window);
  fillTriangle(CanvasTriangle(triangle.vertices[2], midPoint, triangle.vertices[1]), c, window);
}

void drawTriangleFilled(CanvasTriangle triangle, Colour c, DrawingWindow window, float* depthBuffer) 
{
  // Sort verticies
  std::sort(triangle.vertices, triangle.vertices+3, [](CanvasPoint const & a, CanvasPoint const & b) -> bool { return a.y < b.y; } );

  CanvasPoint midPoint = utilities::getTriangleMidPoint(triangle);

  fillTriangle(CanvasTriangle(triangle.vertices[0], triangle.vertices[1], midPoint), c, window, depthBuffer);
  fillTriangle(CanvasTriangle(triangle.vertices[2], midPoint, triangle.vertices[1]), c, window, depthBuffer);
}

void loadImage(PPMImage image, DrawingWindow window) 
{
  for(int y = 0; y < image.height; y++)
  {
    for(int x = 0; x < image.width; x++) 
    {
      Colour pixValue = image.getPixelValueAt(WIDTH*y + x);
      uint32_t colour = (255<<24) + (pixValue.red<<16) + (pixValue.green<<8) + pixValue.blue;
      window.setPixelColour(x, y, colour);
    }
  }
}

void drawTriangleImageFilled(CanvasTriangle triangle, PPMImage image, DrawingWindow window)
{
  // Draw outlined triangle
  Colour c = Colour(255, 255, 255);
  drawLine(triangle.vertices[0], triangle.vertices[1], c, window);
  drawLine(triangle.vertices[1], triangle.vertices[2], c, window);
  drawLine(triangle.vertices[2], triangle.vertices[0], c, window);

  // Sort verticies
  std::sort(triangle.vertices, triangle.vertices+3, [](CanvasPoint const & a, CanvasPoint const & b) -> bool { return a.y < b.y; } );

  CanvasPoint midPoint = utilities::getTriangleMidPoint(triangle);
  drawLine(midPoint, triangle.vertices[1], c, window);

  fillTriangle(CanvasTriangle(triangle.vertices[0], triangle.vertices[1], midPoint), image, window);
  fillTriangle(CanvasTriangle(triangle.vertices[2], midPoint, triangle.vertices[1]), image, window);
}

void fillTriangle(CanvasTriangle triangle, Colour c, DrawingWindow window)
{
  int height = std::ceil( abs(triangle.vertices[0].y - triangle.vertices[1].y) + 1);

  // if(height <= 1) return;

  std::vector<CanvasPoint> from = utilities::interpolateCanvasPoint(triangle.vertices[0], triangle.vertices[1], height);
  std::vector<CanvasPoint> to   = utilities::interpolateCanvasPoint(triangle.vertices[0], triangle.vertices[2], height);

  // std::cout << "Height " << height <<", size:" << from.size() <<  std::endl;

  for(int row=0; row < height; row++) 
  {
    bool reverseOrder = from[row].x > to[row].x;
    CanvasPoint fromPoint = reverseOrder ? to[row] : from[row];
    CanvasPoint toPoint = reverseOrder ? from[row] : to[row];

    int width = toPoint.x - fromPoint.x;

    // std::cout << "Width: " << width << std::endl;

    for(int xOffset = 0; xOffset <= width; xOffset++)
    {
      int x = fromPoint.x + xOffset;
      window.setPixelColour(x, fromPoint.y, c.getPackedInt());
    }
  }
}

void fillTriangle(CanvasTriangle triangle, PPMImage image, DrawingWindow window)
{
  int height = abs(triangle.vertices[0].y - triangle.vertices[1].y) + 1;
  std::vector<CanvasPoint> from = utilities::interpolateCanvasPoint(triangle.vertices[0], triangle.vertices[1], height);
  std::vector<CanvasPoint> to   = utilities::interpolateCanvasPoint(triangle.vertices[0], triangle.vertices[2], height);

  bool reverseFlow = from[1].x > to[1].x;

  for(int row=0; row<height; row++) 
  {
    CanvasPoint fromPoint = reverseFlow ? to[row] : from[row];
    CanvasPoint toPoint = reverseFlow ? from[row] : to[row];

    int width = toPoint.x - fromPoint.x;
    std::vector<CanvasPoint> line = utilities::interpolateCanvasPoint(fromPoint, toPoint, width);

    for(int xOffset = 1; xOffset < width; xOffset++)
    {
      CanvasPoint p = line[xOffset];
      Colour colour = image.getPixelValueAt(p.texturePoint.x, p.texturePoint.y);
      int x = fromPoint.x + xOffset;

      window.setPixelColour(x, fromPoint.y, colour.getPackedInt());
    }
  }
}

void fillTriangle(CanvasTriangle triangle, Colour c, DrawingWindow window, float* depthBuffer)
{
  int height = std::ceil( abs(triangle.vertices[0].y - triangle.vertices[1].y) + 1);

  std::vector<CanvasPoint> from = utilities::interpolateCanvasPoint(triangle.vertices[0], triangle.vertices[1], height);
  std::vector<CanvasPoint> to   = utilities::interpolateCanvasPoint(triangle.vertices[0], triangle.vertices[2], height);

  for(int row=0; row < height; row++) 
  {
    bool reverseOrder = from[row].x > to[row].x;
    CanvasPoint fromPoint = reverseOrder ? to[row] : from[row];
    CanvasPoint toPoint = reverseOrder ? from[row] : to[row];
    
    int y = fromPoint.y + row;
    int width = std::ceil(toPoint.x - fromPoint.x  + 1);
    std::vector<CanvasPoint> line = utilities::interpolateCanvasPoint(fromPoint, toPoint, width);

    for(int col = 0; col < width; col++)
    {
      CanvasPoint p = line[col];
      std::cout << y << " " << p.y << std::endl;

      // int x = std::round(p.x);
      int x = fromPoint.x + col;
      int idx = (y * WIDTH) + x;

      // if(x != x1) {
      //   std::cout << "X: " << p.x << ", X1: " << fromPoint.x + xOffset << std::endl;
      //   std::cout << "X: " << x << ", X1: " << x1 << std::endl;
      // }


      // std::cout << idx << std::endl;

      // window.setPixelColour(x1, y, c.getPackedInt());

      if(idx >= 0 && idx < WIDTH*HEIGHT && depthBuffer[idx] >= p.depth) 
      {
        window.setPixelColour(x, y, c.getPackedInt());
        depthBuffer[idx] = p.depth;
      } else if(idx >= 0 && idx < WIDTH*HEIGHT && depthBuffer[idx] < p.depth)
      {
        std::cout << "DB: " << depthBuffer[idx] << ", PD: " << p.depth << std::endl;
      }
    }
  }
}

void loadModel(OBJFile model, Camera camera, DrawingWindow window, bool showWireframe)
{
  float* depthBuffer = new float[WIDTH*HEIGHT];
  std::fill_n(depthBuffer, WIDTH*HEIGHT, std::numeric_limits<float>::infinity());

  for(int i=9 ; i <= 10; i++) {
      ModelTriangle modelTrangle = model.faces[i];
      CanvasTriangle canvasTriangle = utilities::convertToCanvasTriangle(modelTrangle, camera);

      std::cout << "CanvasTriangle" << canvasTriangle << std::endl;

      if(showWireframe)
        drawTriangleOutline(canvasTriangle, canvasTriangle.colour, window);
      else
        drawTriangleFilled(canvasTriangle, canvasTriangle.colour, window, depthBuffer);
  }

  // for(ModelTriangle modelTriangle: model.faces)
  // {
  //     // std::cout << modelTriangle << std::endl;
  //     CanvasTriangle canvasTriangle = utilities::convertToCanvasTriangle(modelTriangle, camera);

  //     // std::cout << canvasTriangle << std::endl;

  //     if(showWireframe)
  //       drawTriangleOutline(canvasTriangle, canvasTriangle.colour, window);
  //     else
  //       drawTriangleFilled(canvasTriangle, canvasTriangle.colour, window, depthBuffer);
  // }

}
