import math
from typing import List
import contextlib
import io
import matplotlib.pyplot as plt
import matplotlib.ticker as mticker
import ROOT
import numpy as np
from matplotlib.lines import Line2D


def checkRootData(fileName, treeName, branchName):
    try:
        file = ROOT.TFile.Open(fileName)

        # Check if file is open
        if not file or file.IsZombie():
            print("File is Zombie")
            return False

        # Check if tree exists
        if not file.Get(treeName):
            print("Tree does not exist")
            return False

        tree = file.Get(treeName)

        # Check if branch exists
        if not tree.GetBranch(branchName):
            print("Branch name does not exist")
            return False

        branch = tree.GetBranch(branchName)

        # Check if branch is readable
        value = np.zeros(1, dtype=np.float64)
        branch.SetAddress(value)

        if branch.GetEntry(0) < 0:
            print("Branch is not readable")
            return False

        file.Close()

        return True

    except:
        print("An error occurred while checking root data")
        return False
    
def readRootBranch(fileName, treeName, branchName):
    print("fileName:", fileName)

    data = []

    if checkRootData(fileName, treeName, branchName):
        print(checkRootData(fileName, treeName, branchName))
        file = ROOT.TFile.Open(fileName)
        tree = file.Get(treeName)
        branch = tree.GetBranch(branchName)

        value = array('d', [0.0])
        branch.SetAddress(value)

        for i in range(tree.GetEntries()):
            branch.GetEntry(i)
            data.append(value[0])

        file.Close()
    else:
        print("Something is wrong with the file!!!")
        print("Branch:", branchName)

    return data

import os
from os import path
from typing import List
from array import array

def readRootDataFromFolder(folder, treeName, branchName):
    data = []

    # Open folder
    if os.path.isdir(folder):
        # Read all files in folder
        for file_name in os.listdir(folder):
            file_path = os.path.join(folder, file_name)
            # Skip directories
            if os.path.isdir(file_path):
                continue

            # Read data from file
            file_data = readRootBranch(file_path, treeName, branchName)
            data.extend(file_data)

    return data
import matplotlib.pyplot as plt
import math
import numpy as np
from scipy.optimize import curve_fit
import contextlib
import io

def gaussian(x, amp, cen, sigma):
    return amp * np.exp(-0.5 * ((x - cen) / sigma) ** 2)

def sum_of_gaussians(x, amp1, cen1, sigma1, amp2, cen2, sigma2):
    return gaussian(x, amp1, cen1, sigma1) + gaussian(x, amp2, cen2, sigma2)

def fit_histogramG(bin_centers, n):
    # Perform the curve fitting
    p0 = [max(n), np.mean(bin_centers) - np.std(bin_centers)*0.1,
          np.std(bin_centers) * 0.9, max(n)*0.2,
          np.mean(bin_centers) + np.std(bin_centers) , np.std(bin_centers) * 0.5]  # Initial guess for the parameters
    print(p0)
    params, _ = curve_fit(sum_of_gaussians, bin_centers, n, p0=p0)

    # Generate the fitted curve
    fit_curve = sum_of_gaussians(bin_centers, *params)

    return fit_curve, params

def fit_histogram1G(bin_centers, n):
    # Perform the curve fitting
    p0 = [1, np.mean(bin_centers) ,
          np.std(bin_centers) ]  # Initial guess for the parameters
    print(p0)
    params, _ = curve_fit(gaussian, bin_centers, n, p0=p0)

    # Generate the fitted curve
    fit_curve = gaussian(bin_centers, *params)

    return fit_curve, params
def landau(x, amp, cen, sigma):
    return amp * landau.pdf(x, cen, sigma)

def sum_of_landaus(x, amp1, cen1, sigma1, amp2, cen2, sigma2):
    return landau(x, amp1, cen1, sigma1) + landau(x, amp2, cen2, sigma2)

def fit_histogramL(bin_centers, n):
    # Perform the curve fitting
    p0 = [max(n), np.mean(bin_centers) - np.std(bin_centers) * 0.1,
          np.std(bin_centers) * 0.9, max(n) * 0.2,
          np.mean(bin_centers) + np.std(bin_centers), np.std(bin_centers) * 0.5]  # Initial guess for the parameters
    params, _ = curve_fit(sum_of_landaus, bin_centers, n, p0=p0)

    # Generate the fitted curve
    fit_curve = sum_of_landaus(bin_centers, *params)

    return fit_curve, params

def plot_histogram_from_folder(folder, tree_name, branch_name, save_folder, fig, ax, color, legends, limit=0):
    # Read data from root file
    print("Reading X data")
    with contextlib.redirect_stdout(io.StringIO()):
        data1 = readRootDataFromFolder(folder, tree_name, branch_name)
        edep = readRootDataFromFolder(folder, tree_name, "fEdep")

    #print(edep)
    # Filter data based on edep values
    filtered_data1 = [data1[i] for i in range(len(edep)) if edep[i] > 0.5]
    if limit>0:
        filtered_data1 = [filtered_data1[i] for i in range(len(filtered_data1)) if filtered_data1[i] < limit]

    filtered_data1 = [x for x in filtered_data1 if x != float('inf')]
    #print(filtered_data1)

    # Calculate the number of bins using sqrt(n)
    num_bins = int(math.sqrt(len(filtered_data1)))
    print(num_bins)
    #print(num_bins)
    # Calculate the IQR for each distribution
    iqr1 = np.percentile(filtered_data1, 75) - np.percentile(filtered_data1, 25)

    # Calculate the 50th percentile
    percentile_50 = np.percentile(filtered_data1, 50)
    # Plot the histogram with clearer fill and line style
    n, bins, patches = ax.hist(filtered_data1, bins=num_bins, color=color, edgecolor='none', linewidth=1.2, alpha=0.5, density=True, 
                               stacked=True, label=legends+'\nIQR: {:.2f}\n50th percentile: {:.2f}'.format(iqr1, percentile_50))
    # Get the bin centers
    bin_centers = (bins[:-1] + bins[1:]) / 2

    # Perform the curve fitting
    fit_curve, params = fit_histogramG(bin_centers, n)
    #print(params)

    # Plot the fitted curve with a different line style
    ax.plot(bin_centers, fit_curve, color=color, linestyle='--', linewidth=2,label='Fit(I,mean,std): \n{:.2e}, {:.2f}, {:.2f},\n{:.2e}, {:.2f}, {:.2f}'.format(*params))
    return data1

def plot_histogram_from_folderGC3(folder, tree_name, branch_name, save_folder, fig, ax, color, legends, limit=0):
    # Read data from root file
    print("Reading X data")
    with contextlib.redirect_stdout(io.StringIO()):
        data1 = readRootDataFromFolder(folder, tree_name, branch_name)
        edep = readRootDataFromFolder(folder, tree_name, "fEdep")

    #print(edep)
    # Filter data based on edep values
    filtered_data1 = [data1[i] for i in range(len(edep)) if edep[i] > 0.5]
    if limit>0:
        filtered_data1 = [filtered_data1[i] for i in range(len(filtered_data1)) if filtered_data1[i] < limit]

    filtered_data1 = [x for x in filtered_data1 if x != float('inf')]
    #print(filtered_data1)
    # Calculate the number of bins using sqrt(n)
    num_bins = int(math.sqrt(len(filtered_data1)))
    print(num_bins)
    # Calculate the IQR for each distribution
    iqr1 = np.percentile(filtered_data1, 75) - np.percentile(filtered_data1, 25)

    # Calculate the 50th percentile
    percentile_50 = np.percentile(filtered_data1, 50)
    # Plot the histogram with clearer fill and line style
    n, bins, patches = ax.hist(filtered_data1, bins=num_bins, color=color, edgecolor='none', linewidth=1.2, alpha=0.5, density=True, 
                               stacked=True, label=legends+'\nIQR: {:.2f}\n50th percentile: {:.2f}'.format(iqr1, percentile_50))
    # Get the bin centers
    bin_centers = (bins[:-1] + bins[1:]) / 2

    # Perform the curve fitting
    fit_curve, params = fit_histogram1G(bin_centers, n)
    #print(params)

    # Plot the fitted curve with a different line style
    ax.plot(bin_centers, fit_curve, color=color, linestyle='--', linewidth=2,label='Fit(I,mean,std): \n{:.2e}, {:.2f}, {:.2f}\n{:.2e}, {:.2f}, {:.2f}'.format(*params))
    return data1