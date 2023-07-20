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
    

def readRootBranch(fileName, treeName, branchName, data_type):
    print("fileName:", fileName)

    data = []

    if checkRootData(fileName, treeName, branchName):
        print(checkRootData(fileName, treeName, branchName))
        file = ROOT.TFile.Open(fileName)
        tree = file.Get(treeName)
        branch = tree.GetBranch(branchName)

        if data_type == 'int':
            value = array('i', [0])
        elif data_type == 'double':
            value = array('d', [0.0])
        else:
            print("Invalid data type specified!")
            return []

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

def readRootDataFromFolder(folder, treeName, branchName, datatype='double'):
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
            file_data = readRootBranch(file_path, treeName, branchName,datatype)
            data.extend(file_data)

    return data

#############
## LC 1 folder
#############

def box_plot_1xLC(folder: str, output_filename: str):
    # Define the variables
    treeName1 = "EndOfEvent"
    branchName1 = "fPrimaryZ"
    treeName2 = "EndOfEvent"
    branchName2 = "fphL"
    treeName3 = "EndOfEvent"
    branchName3 = "fphR"
    xMin = -29
    xMax = 29
    yMin = 0
    yMax = 6000
    saveFolder = "CSV/"
    treeName3 = "EndOfEvent"
    branchName4 = "fDdep"
    textwidth = 16.51

    # Create a figure and axes
    fig, ax = plt.subplots(figsize=(textwidth * 0.45, textwidth * 0.4))

    # Read data from root file
    print("Reading X data")
    with contextlib.redirect_stdout(io.StringIO()):
        data1 = readRootDataFromFolder(folder, treeName1, branchName1)
        data2 = readRootDataFromFolder(folder, treeName2, branchName2)
        data3 = readRootDataFromFolder(folder, treeName3, branchName3)

    #print(len(data1))
    #print(len(data2))

    n = int(np.ceil(np.sqrt(len(data1))))  # Number of ranges
    #print(n)
    range_size = xMax * 2 / n

    # Create a list to store the data within each range
    data_in_ranges = []
    box_positions = []
    box_widths = []

    # Iterate over each range and extract the data within it
    for i in range(n):
        range_min = xMin / 1000 + i * range_size / 1000
        range_max = range_min + range_size / 1000
        values_in_range = []
        for j in range(len(data1)):
            if range_min <= data1[j] < range_max:
                if data2[j] > 0 and data3[j] > 0:
                    values_in_range.append((data3[j] + data2[j]) / 2)
        data_in_ranges.append(values_in_range)

        # Calculate the box position and width
        box_position = (range_min + range_max) / 2 * 1000
        box_width = 0.9 * (range_max - range_min) * 1000
        box_positions.append(box_position)
        box_widths.append(box_width)

    # Set the y-axis limits
    #print(box_positions)

    # Plot the box plot for each range
    boxplot = ax.boxplot(data_in_ranges, positions=box_positions, widths=box_widths, showfliers=True,
                         patch_artist=False,  # Set patch_artist to False
                         whiskerprops=dict(color='blue'), capprops=dict(color='blue'),
                         medianprops=dict(color='blue'), flierprops=dict(marker='o', markersize=3,
                                                                         markerfacecolor='black'), whis=1.5)


    # Set the x-axis limits
    ax.set_xlim(-30, 30)

    # Round the x-axis tick labels to the first decimal place
    ax.xaxis.set_major_formatter(plt.FuncFormatter(lambda x, _: f'{x:.1f}'))
        # Add a grid
    ax.grid(True, axis='y', linestyle='--', color='lightgray')
    # Add labels


    # Customize the color of all boxes
    for box in boxplot['boxes']:
        box.set_color('blue')

    # Set the number of divisions on the x-axis
    num_divisions = 10  # Choose the desired number of divisions
    ax.xaxis.set_major_locator(mticker.MaxNLocator(num_divisions))

    # Calculate the mean of each box
    box_means = [np.mean(data) for data in data_in_ranges]
    ax.plot(box_positions, box_means, 'ro', markersize=5)

    # Remove the title
    ax.set_title('')

    x=box_positions
    y=box_means
    x_filtered = [x_val for x_val, y_val in zip(x, y) if not math.isnan(y_val)]
    y_filtered = [y_val for y_val in y if not math.isnan(y_val)]

    #print(x)
    #print(y)
    #print(x_filtered)
    #print(y_filtered)

    # Add a parabolic fit to the mean values
    x_fit = np.linspace(min(x_filtered), max(x_filtered), 100)
    y_fit = np.polyval(np.polyfit(x_filtered, y_filtered, 2), x_fit)
    ax.plot(x_fit, y_fit, color='red', linestyle='--')
    
    # Get the coefficients from the polynomial fit
    coefficients = np.polyfit(x_filtered, y_filtered, 2)
    a, b, c = coefficients
    #print(x_fit)
    #print(y_fit)

    # Increase the font size of the axes labels
    plt.xlabel(r'Length [\si{\milli\meter}]')
    plt.ylabel(r'Light Collection [ph]')

    # Increase the font size of the tick labels
    plt.xticks(fontsize=18)
    plt.yticks(fontsize=18)
    
    y_predicted = np.polyval(coefficients, x_filtered)

    # Calculate the residuals
    residuals = (y_filtered - y_predicted)/len(y_filtered)
    print(residuals)
    # Calculate the mean squared error (MSE)
    mse = np.mean(residuals**2)
    
    # Customize the legend
    legend_elements = [
    Line2D([0], [0], marker='o', color='black', markersize=5, label='Outliers', linestyle='None'),
    Line2D([0], [0], marker='o', color='red', markersize=5, label='Mean', linestyle='None'),
    Line2D([0], [0], color='red', linestyle='--', label=r'$\mathrm{Fit: } %.1fx^2 + %.1fx + %.1f$' % (a, b, c)),
    Line2D([0], [0], color='black', linestyle='', label=r'$\mathrm{MSE}\mathrm{:}\ %.2f$' % mse)
    ]

    # Add the legend to the plot
    ax.legend(handles=legend_elements, loc='upper right',fontsize=18)

    # Save the figure as a PDF
    plt.savefig(output_filename, format='pdf', bbox_inches='tight')

    # Show the plot
    plt.show()
    return
    
#############
## LO 1 folder
#############


def box_plot_1xLO(folder: str, output_filename: str, npol: int):
    # Define the variables
    treeName1 = "EndOfEvent"
    branchName1 = "fPrimaryZ"
    treeName2 = "EndOfEvent"
    branchName2 = "fphL"
    treeName3 = "EndOfEvent"
    branchName3 = "fphR"
    xMin = -29
    xMax = 29
    yMin = 0
    yMax = 6000
    saveFolder = "CSV/"
    treeName3 = "EndOfEvent"
    branchName4 = "fDdep"
    geometry_textwidth = 384  # pt
    pt_to_inch = 1.0 / 72.27  # Conversion factor from points to inches
    plot_width = geometry_textwidth * pt_to_inch *0.5
    fig, ax = plt.subplots(figsize=(plot_width, plot_width*0.9))

    # Read data from root file
    print("Reading X data")
    with contextlib.redirect_stdout(io.StringIO()):
        data1 = readRootDataFromFolder(folder, treeName1, branchName1)
        data2 = readRootDataFromFolder(folder, treeName2, branchName2)
        data3 = readRootDataFromFolder(folder, treeName3, branchName3)
        edep = readRootDataFromFolder(folder, treeName1, "fEdep")


    n = int(np.ceil(np.sqrt(len(data1))))  # Number of ranges
    range_size = xMax * 2 / n

    # Create a list to store the data within each range
    data_in_ranges = []
    box_positions = []
    box_widths = []

    # Iterate over each range and extract the data within it
    for i in range(n):
        range_min = xMin / 1000 + i * range_size / 1000
        range_max = range_min + range_size / 1000
        values_in_range = []
        for j in range(len(data1)):
            if range_min <= data1[j] < range_max:
                if data2[j] > 0 and data3[j] > 0:
                    if edep[j]>0.1:
                        val=(data3[j] + data2[j]) / 2 / edep[j]
                        if(val>1000):
                            if(val<3000):
                                values_in_range.append((data3[j] + data2[j]) / 2 / edep[j])
        data_in_ranges.append(values_in_range)

        # Calculate the box position and width
        box_position = (range_min + range_max) / 2 * 1000
        box_width = 0.9 * (range_max - range_min) * 1000
        box_positions.append(box_position)
        box_widths.append(box_width)

    # Set the y-axis limits
    print(box_positions)

    # Plot the box plot for each range

    boxplot = ax.boxplot(data_in_ranges, positions=box_positions, widths=box_widths, showfliers=True,
                         patch_artist=False,  # Set patch_artist to False
                         whiskerprops=dict(color='blue', linewidth=0.5), capprops=dict(color='blue', linewidth=0.5),
                         medianprops=dict(color='blue', linewidth=0.5), flierprops=dict(marker='o', markersize=1,
                                                                         markerfacecolor='black'), whis=1.5,boxprops=dict(color='blue', linewidth=0.5))
    # Set the y-axis label
    ax.set_ylabel("Value")

    # Set the plot title
    ax.set_title("Box Plot of Data in Each Range")

    # Set the x-axis limits
    ax.set_xlim(-30, 30)

    # Round the x-axis tick labels to the first decimal place
    ax.xaxis.set_major_formatter(plt.FuncFormatter(lambda x, _: f'{x:.1f}'))
        # Add a grid
    ax.grid(True, axis='y', linestyle='--', color='lightgray')
    # Add labels
    ax.set_xlabel(r"Length $\left(\si{\milli\meter}\right)$")
    ax.set_ylabel(r"Light Output $\left(\si{\sum\gamma_d/\mega\electronvolt}\right)$")

    # Customize the color of all boxes
    for box in boxplot['boxes']:
        box.set_color('blue')

    # Set the number of divisions on the x-axis
    num_divisions = 6  # Choose the desired number of divisions
    ax.xaxis.set_major_locator(mticker.MaxNLocator(num_divisions))

    # Calculate the mean of each box
    box_means = [np.mean(data) for data in data_in_ranges]
    ax.plot(box_positions, box_means, 'ro', markersize=1)

    # Remove the title
    ax.set_title('')

    x=box_positions
    y=box_means
    x_filtered = [x_val for x_val, y_val in zip(x, y) if not math.isnan(y_val)]
    y_filtered = [y_val for y_val in y if not math.isnan(y_val)]

    # Add a parabolic fit to the mean values
    x_fit = np.linspace(min(x_filtered), max(x_filtered), 100)
    y_fit = np.polyval(np.polyfit(x_filtered, y_filtered, npol), x_fit)
    ax.plot(x_fit, y_fit, color='red', linestyle='--')
    
    # Calculate residuals based on the parabolic fit
    residuals = y_filtered - np.polyval(np.polyfit(x_filtered, y_filtered, npol), x_filtered)
    # Calculate ss_res and ss_tot based on the parabolic fit
    ss_res = np.sum(residuals**2)
    ss_tot = np.sum((y_filtered - np.mean(y_filtered))**2)

    # Calculate R2
    r2 = 1 - (ss_res / ss_tot)
    # Increase the font size of the axes labels

    
        # Customize the legend
    legend_elements = [
    Line2D([0], [0], marker='o', color='black', markersize=3, label='Outliers', linestyle='None'),
    Line2D([0], [0], marker='o', color='red', markersize=3, label='Mean', linestyle='None'),
    Line2D([0], [0], color='red', linestyle='--', label='Fit, R2 {:.2f} \n{:.2e}x² + {:.2f}x + {:.2f}'.format(r2, *np.polyfit(x_filtered, y_filtered, npol)))
    ]


    
    # Add the legend to the plot
    ax.legend(handles=legend_elements, loc='upper right')

    # Save the figure as a PDF
    plt.savefig(output_filename, format='pdf', bbox_inches='tight')

    # Show the plot
    plt.show()
    return