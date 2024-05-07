#include <math.h>
#include <sys/stat.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <random>
#include <vector>

namespace congweave {

#define WORKLOAD_CDF_GRID (1000)

class CustomRand {
   private:
    std::vector<std::pair<uint32_t, double> > cdf;
    bool grid = false;

   public:
    CustomRand() {}
    ~CustomRand() {}

    inline bool exists_test(char* name) {
        struct stat buffer;
        return (stat(name, &buffer) == 0);
    }

    // void loadCdf(char* cdf_dir) {
    //     std::ifstream cdf_file;
    //     /* check file exists */
    //     if (!exists_test(cdf_dir)) {
    //         printf("ERROR - CDF File does not exist: %s\n", cdf_dir);
    //         exit(1);
    //     }

    //     /* open file and get cdf */
    //     cdf_file.open(cdf_dir);
    //     uint32_t size;
    //     double cdf_val;
    //     while (!cdf_file.eof()) {
    //         cdf_file >> size >> cdf_val;
    //         // std::cout << "Size: " << size << ", cdf: " << cdf_val << std::endl;
    //         cdf.push_back(std::make_pair(size, cdf_val));
    //         fflush(stdout);
    //     }
    //     cdf_file.close();
    // }

    void loadCdfGrid(char* cdf_dir) {
        this->grid = true;
        std::ifstream cdf_file;
        /* check file exists */
        if (!exists_test(cdf_dir)) {
            printf("ERROR - CDF File does not exist: %s\n", cdf_dir);
            exit(1);
        }

        /* open file and get cdf */
        cdf_file.open(cdf_dir);
        cdf.resize(WORKLOAD_CDF_GRID + 1);
        uint32_t size;
        double cdf_val, prev_fs = 0.;
        int cursor = 0;
        while (!cdf_file.eof()) {
            cdf_file >> size >> cdf_val;
            int current = (int)(cdf_val * WORKLOAD_CDF_GRID * 1.0 / 100);
            for (int i = cursor; i < current; i++) {
                cdf[i].first = prev_fs + (size - prev_fs) * (i - cursor) / ((double)(current - cursor));
                cdf[i].second = i * (1.0 / WORKLOAD_CDF_GRID);
                // printf("Size: %u, cdf: %f\n", cdf[i].first, cdf[i].second); /** DEBUGGING **/
            }
            cursor = current;
            prev_fs = size;
            fflush(stdout);
        }
        for (int i = cursor; i < WORKLOAD_CDF_GRID + 1; i++) {
            cdf[i].first = size;
            cdf[i].second = i * (1.0 / WORKLOAD_CDF_GRID);
            // printf("Size: %u, cdf: %f\n", cdf[i].first, cdf[i].second); /** DEBUGGING **/
        }
        std::cout << "Finished loading CDF: " << cdf_dir << std::endl;
        cdf_file.close();
    }

    double getAvg() {
        uint32_t last_x = cdf.front().first;
        double last_y = cdf.front().second;
        double ret = 0;
        uint32_t n_iter = 0;
        for (auto iter : cdf) {
            if (n_iter == 0) {  // bypass first iter
                n_iter++;
                continue;
            }
            uint32_t x = iter.first;
            double y = iter.second;
            ret += (x + last_x) / 2.0 * (y - last_y);
            last_x = x;
            last_y = y;
        }
        return ret;
    }

    double getValueFromPercentile(double p) {
        uint32_t x0, x1;
        double y0, y1;
        for (uint32_t i = 1; i < cdf.size(); i++) {
            if (p <= cdf[i].second) {
                x0 = cdf[i - 1].first;
                y0 = cdf[i - 1].second;
                x1 = cdf[i].first;
                y1 = cdf[i].second;
                if (y1 == y0) {
                    return x1;
                } else {
                    return x0 + (x1 - x0) / (y1 - y0) * (p - y0);
                }
            }
        }
        return x1;
    }

    uint32_t randgen() {
        uint32_t rand_val = (this->grid) ? (rand() % 101) : (rand() % 10001);  // depending on cdf range ~ 1.0 or 100% (grid: 1.0)
        uint32_t msg_size = static_cast<uint32_t>(getValueFromPercentile(rand_val * 0.01));
        msg_size = (msg_size > 0) ? msg_size : 1;
        return msg_size;
    }


    static double poisson(double inter_arrival_rate) {
        double rand_val = rand() * 1.0 / RAND_MAX;
        return std::log(1 - rand_val) * (-1.0) * (inter_arrival_rate);
    }

};
}  // namespace congweave