#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <Colour.h>

class PPMImage {

    private:
        std::string path;
        int maxValue;
        std::vector<Colour> payload;

        bool readPPMFile(std::ifstream &imageFile) 
        {
            std::string line;
            
            // Check the version
            std::getline(imageFile, line);
            if(line != "P6")
            {
                std::cout << "The version of the PPM file is not P6!" << std::endl;
                return false;
            }

            // Skip comment
            std::getline(imageFile, line);
            while (line[0] == '#') {
                std::getline(imageFile, line);
            }

            // Get the dimensions of the image
            std::stringstream dimensions(line);
            try {
                dimensions >> this -> width;
                dimensions >> this -> height;
            } catch (std::exception &e) {
                std::cout << "Header file format error. " << e.what() << std::endl;
                return false;
            }

            // Get max value
            std::getline(imageFile, line);
            std::stringstream max_val(line);
            try {
                max_val >> this -> maxValue;
            } catch (std::exception &e) {
                std::cout << "Header file format error. " << e.what() << std::endl;
                return false;
            }

            // Get the pixel values
            int size = this->width * this->height;
            // this->payload.reserve(size);
            int r, g, b;

            for (unsigned int i = 0; i < size; ++i) {
                r = imageFile.get();
                g = imageFile.get();
                b = imageFile.get();

                Colour pixelValue = Colour( r, g, b );
                this->payload.push_back(pixelValue);
            }


            return true;
        }

    public:
        int width, height;
        PPMImage(std::string path) 
        {
            this->path = path;
            std::ifstream imageFile(path);

            if (imageFile.is_open()) 
            {
                std::cout << "Image is opened" << std::endl;
                bool isSuccessful = readPPMFile(imageFile); 

                if(isSuccessful)
                    std::cout << "Width: " << this->width << ", Height: " << this->height 
                    << ", Max: " << this->maxValue << ", Payload size: " << this->payload.size() << std::endl;
                else
                    std::cout << "Could not read image" << std::endl;
            }
        }

        Colour getPixelValueAt(int index) 
        {
            if(index >= this->width * this->height) 
            {
                std::cout << "Requested pixel is out of bounds." << std::endl;
                return Colour();
            }

            return this->payload[index];
        }

        Colour getPixelValueAt(int x, int y) 
        {
            if(x > width || x < 0 || y > height || y < 0) 
            {
                std::cout << "Requested image pixel out of scope" << std::endl;
                return Colour(0,0,0);
            }

            int index = width * y + x;
            
            if(index >= this->width * this->height) 
            {
                std::cout << "Requested pixel is out of bounds." << std::endl;
                return Colour();
            }

            return this->payload[index];
        }

};