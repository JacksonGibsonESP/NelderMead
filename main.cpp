#include <iostream>
#include <random>
#include <chrono>
#include <cstring>

double f(double *x) {
    return (x[0] * x[0] + x[1] - 11) * (x[0] * x[0] + x[1] - 11) + (x[0] + x[1] * x[1] - 7) * (x[0] + x[1] * x[1] - 7);
}

double variance(double **x, int n) {
    double *m = new double[n]();
    for (int i = 0; i < n + 1; i++) {
        for (int j = 0; j < n; j++) {
            m[j] += x[i][j] / (n + 1);
        }
    }
    double *d = new double[n]();
    for (int i = 0; i < n + 1; i++) {
        for (int j = 0; j < n; j++) {
            d[j] += (x[i][j] - m[j]) * (x[i][j] - m[j]) / (n + 1);
        }
    }
    double sum = 0;
    for (int i = 0; i < n; i++) {
        sum += d[i];
    }
    return sum;
}

int main() {
    int iteration = 0;
    double alpha = 1;
    double beta = 0.5;
    double gamma = 2;
    int n = 2;

    // 1 initialize simplex
    long long int seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_real_distribution<double> distribution(-4, 4);
    double **x = new double*[n + 1];
    for (int i = 0; i < n + 1; i++) {
        x[i] = new double[n];
        for (int j = 0; j < n; j++) {
            x[i][j] = distribution(generator);
        }
    }

    double *y = new double[n + 1];
    double *x_h = x[0];
    int x_h_index = 0;
    double *x_g;
    double *x_l = x[0];
    double *x_c = new double[n]();
    double *x_r = new double[n];
    double *x_e = new double[n];
    double *x_s = new double[n];

    while (variance(x, n) > 0.0000000001) {
        ++iteration;
        std::cout << iteration << " " << x[0][0] << " " << x[0][1] << " " << f(x[0]) << "\n";
        // 2
        for (int i = 0; i < n + 1; i++) {
            y[i] = f(x[i]);
            if (f(x_l) > y[i]) {
                x_l = x[i];
            }
            if (f(x_h) < y[i]) {
                x_h = x[i];
                x_h_index = i;
            }
        }
        x_g = x_l;
        for (int i = 0; i < n + 1; i++) {
            if (i == x_h_index) {
                continue;
            }
            if (f(x_g) < y[i]) {
                x_g = x[i];
            }
        }

        // 3
        for (int i = 0; i < n + 1; i++) {
            if (x[i] == x_h) {
                continue;
            }
            for (int j = 0; j < n; j++){
                x_c[j] += x[i][j] / n;
            }
        }

        // 4
        for (int i = 0; i < n; i++){
            x_r[i] = (1 + alpha) * x_c[i] - alpha * x_h[i];
        }

        // 5
        if (f(x_r) < f(x_l)) {
            for (int i = 0; i < n; i++){
                x_e[i] = (1 - gamma) * x_c[i] + gamma * x_r[i];
            }
            if (f(x_e) < f(x_r)){
                memcpy(x_h, x_e, sizeof(double) * n);
            } else {
                memcpy(x_h, x_r, sizeof(double) * n);
            }
            continue;
        } else if (f(x_l) < f(x_r) && f(x_r) < f(x_g)){
            memcpy(x_h, x_r, sizeof(double) * n);
            continue;
        } else if (f(x_g) < f(x_r) && f(x_r) < f(x_h)) {
//            double *tmp = x_r;
//            x_r = x_h;
//            x_h = tmp;
            memcpy(x_h, x_r, sizeof(double) * n);
        }

        // 6
        for (int i = 0; i < n; i++){
            x_s[i] = beta * x_h[i] + (1 - beta) * x_c[i];
        }
        // 7
        if (f(x_s) < f(x_h)) {
            memcpy(x_h, x_s, sizeof(double) * n);
            continue;
        } else { // 8
            for (int i = 0; i < n + 1; i++) {
                if (x[i] == x_l) {
                    continue;
                }
                for (int j = 0; j < n; j++) {
                    x[i][j] = x_l[j] + (x[i][j] - x_l[j]) / 2;
                }
            }
        }
    }
    std::cout << iteration << "\n";
    return 0;
}
