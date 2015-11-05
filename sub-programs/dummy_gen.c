#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define TIME_SPAN (1.0/(12*30))

typedef struct {
    float x, y, z;
    double theta;
    double e, l;
    double speed;
} planet;


int nextPosition(planet &p) {
    p.theta += TIME_SPAN * 2*M_PI / p.speed;
    if (p.theta > 2*M_PI) p.theta -= 2*M_PI;

    double r = p.l / (1 + cos(p.theta)*(p.e));
    p.x = r * cos(p.theta);
    p.y = 0;
    p.z = r * sin(p.theta);
    return 0;
}

int main(void) {
    planet planets[10];
    planets[0].theta = 0;
    planets[0].speed = 1;
    planets[0].e = 0;
    planets[0].l = 0;
    planets[1].theta = 0;
    planets[1].speed = 0.241;
    planets[1].e = 0.20563593;
    planets[1].l = 0.38709927;
    planets[2].theta = 0;
    planets[2].speed = 0.615;
    planets[2].e = 0.00677672;
    planets[2].l = 0.72333566;
    planets[3].theta = 0;
    planets[3].speed = 1.000;
    planets[3].e = 0.01671123;
    planets[3].l = 1.00000261;
    planets[4].theta = 0;
    planets[4].speed = 1.881;
    planets[4].e = 0.09339410;
    planets[4].l = 1.52371034;
    planets[5].theta = 0;
    planets[5].speed = 11.86;
    planets[5].e = 0.04838624;
    planets[5].l = 5.20288700;
    planets[6].theta = 0;
    planets[6].speed = 29.46;
    planets[6].e = 0.05386179;
    planets[6].l = 9.53667594;
    planets[7].theta = 0;
    planets[7].speed = 84.01;
    planets[7].e = 0.04725744;
    planets[7].l = 19.18916464;
    planets[8].theta = 0;
    planets[8].speed = 164.79;
    planets[8].e = 0.00859048;
    planets[8].l = 30.06992276;
    planets[9].theta = 0;
    planets[9].speed = 247.74;
    planets[9].e = 0.25025;
    planets[9].l = 39.445;

    for(int i=0; i<10; i++) {
        planet &p = planets[i];
        char fname[20];
        sprintf(fname, "planet_xyz_no%d.bpa", i);
        FILE *fp = fopen(fname, "wb");
        float _t = TIME_SPAN;
        fwrite(&_t, sizeof(float), 1, fp);
        for (int t=0; t<1000000; t++) {
            nextPosition(p);
            fwrite(&p.x, sizeof(float), 1, fp);
            fwrite(&p.z, sizeof(float), 1, fp);
            fwrite(&p.y, sizeof(float), 1, fp);
        }
        fflush(fp);
        fclose(fp);
    }
    return 0;
}
