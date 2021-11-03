#pragma once
#include "ofMain.h"
#include "ofThread.h"
#include <complex>
#include <vector>

class FractalGenerator : public ofThread {
    public:

        class ZeroDerivative : public std::exception
        {
        };

        std::complex<double> Function(std::complex<double> z0)
        {
            return fparser.Eval(&z0);
        }

        std::complex<double> IntegralFunction(double phi, std::complex<double> z0, double r)
        {
            return Function(z0 + r * exp(1i * phi)) * exp(-1i * phi) / r;
        }

        std::complex<double> Derivative(std::complex<double> z0)
        {
            //double h =/* std::pow(*/eps/*, 1 / 5.)*/;
            //std::complex<double> integral = 0;
            //for (double phi = 0; phi < 2 * PI; phi += h)
            //    integral += /*7. * */IntegralFunction(phi, z0, eps)/* +
            //    32. * IntegralFunction(phi + 1 / 4 * h, z0, eps) +
            //    12. * IntegralFunction(phi + 2 / 4 * h, z0, eps) +
            //    32. * IntegralFunction(phi + 3 / 4 * h, z0, eps) +
            //    7. * IntegralFunction(phi + h, z0, eps)*/;
            //integral *= h / (/*90. **/ 2. * PI);
            //return integral;
            return (Function(z0 + eps) - Function(z0 - eps)) / (2 * eps);
        }

        std::complex<double> NextPoint(std::complex<double> z0)
        {
            std::complex<double> dz = Derivative(z0);
            if (std::abs(dz) < eps)
                throw ZeroDerivative();
            return z0 - Function(z0) / dz;
        }

        std::pair<int, int> GetNumberRoot(std::complex<double> z0)
        {
            std::complex<double> curr = z0;
            size_t j = 0;
            try
            {
                std::complex<double> next = NextPoint(z0);
                while (std::abs(next - curr) > eps)
                {
                    curr = next;
                    next = NextPoint(next);
                    j++;
                    if (j > 1e3)
                        return { -1, -1 };
                }
            }
            catch (ZeroDerivative& ex)
            {
                return { -1, -1 };
            }
            size_t i = 0;
            for (auto el : roots)
            {
                if (std::abs(el - curr) < 2*eps)
                {
                    return { i, j };
                }
                i++;
            }
            roots.push_back(curr);
            return { roots.size() - 1, j };
        }

        void SetSingularity(std::complex<double> z0)
        {
            double r = sqrt(3) * a / this->width;
            for (auto el : singularities)
                if (std::abs(el - z0) < 2 * r)
                    return;
            double h = 0.1;
            std::complex<double> integral = 0;
            for (double phi = 0; phi < 2 * PI; phi += h)
                integral += Function(z0 + r * exp(1i * phi)) * exp(1i * phi);
            if (std::abs(integral) > h)
                singularities.push_back(z0);
        }

        void setup(int width, int height, double eps, string function, double a, double quality)
        {
            this->width = width * quality / 100.;
            this->height = height * quality / 100.;
            image.allocate(this->width, this->height, OF_IMAGE_COLOR);
            this->eps = eps;
            this->fparser.Parse(function, "z");
            this->a = a;
            this->wait = 0;
            roots.clear();
            singularities.clear();
        }

        void threadedFunction()
        {
            std::cout << this->width << " : " << this->height << std::endl;
            double y = a;
            for (auto line : image.getLines()) {
                double x = -a;
                for (auto pixel : line.getPixels()) {
                    auto order_count = GetNumberRoot({x, y});
                    //SetSingularity({x, y});
                    ofColor color;
                    if (order_count.first == -1 && order_count.second == -1)
                        color = ofColor::black;
                    else if (order_count.first < colors.size())
                        color = colors[order_count.first];
                    else
                        color = ofColor(std::max(230 - order_count.first, 0));
                    pixel[0] = color.r;
                    pixel[1] = color.g;
                    pixel[2] = color.b;
                    x += 2.*a / width;
                    wait += 1. / (this->width * this->height);
                }
                y -= 2. * a / height;
            }
        }

        ofPixels image;
        int width;
        int height;
        FunctionParser_cd fparser;
        double eps;
        double a;
        std::vector<std::complex<double>> roots;
        std::vector<std::complex<double>> singularities;
        double wait;
        std::vector<ofColor> colors =
        {
            ofColor(240,163,255),ofColor(0,117,220),ofColor(153,63,0),ofColor(76,0,92),ofColor(0,92,49),ofColor(43,206,72),ofColor(255,204,153),ofColor(148,255,181),ofColor(143,124,0),ofColor(157,204,0),ofColor(194,0,136),ofColor(0,51,128),ofColor(255,164,5),ofColor(255,168,187),ofColor(66,102,0),ofColor(255,0,16),ofColor(94,241,242),ofColor(0,153,143),ofColor(224,255,102),ofColor(116,10,255),ofColor(153,0,0),ofColor(255,255,128),ofColor(255,255,0),ofColor(255,80,5)
        };
};