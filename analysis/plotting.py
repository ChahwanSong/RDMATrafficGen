#!/usr/bin/python3

from genericpath import exists
import subprocess
import os
import time
from xmlrpc.client import boolean
import numpy as np
import copy
import shutil
import random
from datetime import datetime
import sys
import os
import argparse
from datetime import date
import glob
from collections import Counter

import matplotlib as mpl
import matplotlib.pyplot as plt

mpl.rcParams["legend.frameon"] = True
mpl.rcParams['pdf.fonttype'] = 42

colors = mpl.cm.tab10(range(10))
# colors = ['#00429d', '#4b6caf', '#7a99bf', '#d9b9c2', '#ebaea3', '#bb686d', '#6c3942']
# colors = ['#00429d', '#4771b2', '#73a2c6', '#a5d5d8', '#ffffe0', '#ffbcaf', '#f4777f', '#cf3759', '#93003a']
markers = ['.', 'x', '^', 'o', '>', '+', 'p']


def getFilePath():
    dir_path = os.path.dirname(os.path.realpath(__file__))
    print("File directory: {}".format(dir_path))
    return dir_path


def cdf_plot(filenames: list, output_dir: str, target_loads:list, folder: str):
    
    for target_load in target_loads:
        print("Plotting {}".format(folder))
        fig = plt.figure(figsize=(3.7, 2.5), facecolor='white')
        ax = fig.add_subplot(111)
        for filename in filenames:
            parsed_filename = filename.split("/")[-1].replace("\n", "").split("_")
            lb_mode = parsed_filename[0]
            workload = parsed_filename[1]
            load = parsed_filename[2]
            if float(load) != float(target_load):
                continue
            dcqcn = parsed_filename[3]
            pfc = parsed_filename[4]
            runtime = parsed_filename[5]
            param = ""
            if parsed_filename[6] != "fct":
                param = parsed_filename[6]

            legend = "{lb_mode}".format(lb_mode=lb_mode)
            title = "{workload}_{load}_{dcqcn}_{pfc}".format(
                workload=workload, load=load, dcqcn=dcqcn, pfc=pfc)

            lines = open(filename, "r").readlines()
            x_arr = []
            y_arr = []
            alpha = 1.0
            for line in lines:
                parsed_line = line.replace("\n", "").split(" ")
                x_val = float(parsed_line[0])
                x_arr.append(x_val)
                y_val = float(parsed_line[3])
                y_arr.append(y_val)

            if "ecmp" in filename:    
                linestyle = '-'
                linewidth = 2.5
                color = colors[0]
                alpha = 0.3
            elif "letflow" in filename:
                linestyle = '--'
                linewidth = 2
                color = colors[1]
                legend += "({}us)".format(param)
            elif "drill" in filename:
                linestyle = ':'
                linewidth = 2
                color = colors[2]
            elif "conweave" in filename:
                linestyle = '--'
                linewidth = 1.5
                color = colors[3]
                legend += "({}us)".format(param)
            

            ax.plot(x_arr,
                    y_arr,
                    linestyle=linestyle,
                    linewidth=linewidth,
                    color=color,
                    # marker=markers[lbmode_to_idx[lbmode]],
                    # markersize=4,
                    label="{}".format(legend))

        ax.spines['top'].set_visible(False)
        ax.spines['right'].set_visible(False)
        ax.yaxis.set_ticks_position('left')
        ax.xaxis.set_ticks_position('bottom')

        ax.set_xscale('log', basex=10)
        # ax.set_yscale('log', base=10)

        font_size = 9
        ax.set_xlabel("Flow Completion Time (us)", fontsize=font_size)
        ax.set_ylabel("CDF", fontsize=font_size)

        plt.xticks(fontsize=font_size)
        plt.yticks(fontsize=font_size)

        # ax.set_xlim(10, 1200)
        # ax.set_xticks(x_ticks)
        # ax.set_xticklabels(x_ticks)
        # ax.set_yticks([0, 10, 20, 30, 40, 50])
        # ax.set_yticklabels(['0%', '10%', '20%', '30%', '40%', '50%'], fontsize=12)
        # ax.get_xaxis().set_major_formatter(mpl.ticker.ScalarFormatter())

        ax.legend(loc="lower right", fontsize=font_size, facecolor='white', ncol=1, framealpha=0.8)
        # ax.legend(bbox_to_anchor=(1.04, 0.5), loc="center left", borderaxespad=0,
        #         fontsize=10.5, facecolor='white', ncol=1, framealpha=0.9,
        #         labelspacing=0.3, columnspacing=0.3) # frameon=False
        
        fig.tight_layout()
        plt.grid(linestyle='--', color='gray', linewidth=0.05)
        ax.grid(which='minor', alpha=0.2)
        ax.grid(which='major', alpha=0.5)

        plt.savefig("{}/{}_L{}.pdf".format(output_dir, title, load), transparent=False, bbox_inches='tight')
        plt.close()


def main():
    parser = argparse.ArgumentParser(description='plotting CDF')
    parser.add_argument('-d', dest='dir', action='store',
                        default='output', help="output folder to plot all")
    args = parser.parse_args()

    file_dir = getFilePath()
    output_dir = "{}/{}".format(file_dir, args.dir)
    print("output directory - {}".format(output_dir))
    # make directory if not exists
    isExist = os.path.exists("{}/{}".format(file_dir, args.dir))
    if not isExist:
        # os.makedirs(file_dir + "/plot/")
        # print("The new directory is created - {}".format(file_dir + "/plot/"))
        print("Directory does not exist - {}".format(output_dir))
        exit(1)

    allFiles = glob.glob("{}/*".format(output_dir))

    # collect only CDF files
    target_loads = ["0.5", "0.8"]
    cdf_files = []
    for filename in allFiles:
        if "fct_all.dat" in filename:
            cdf_files.append(filename)

    cdf_plot(cdf_files, output_dir, target_loads, args.dir)


if __name__ == "__main__":
    main()

