#pragma once
#include "ofMain.h"

class ImageLoader : public ofThread {
    ImageLoader() {
        loaded = false;
    }
    void setup(string imagePath) {
        this->path = imagePath;
    }

    void threadedFunction() {
        ofLoadImage(image, path);
        loaded = true;
    }

    ofPixels image;
    string path;
    bool loaded;
}