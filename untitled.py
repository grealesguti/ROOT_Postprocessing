import math
from typing import List
import contextlib
import io
import matplotlib.pyplot as plt


folder="../TierIIData/2023_02/Final_NSGAII_375x3"
folder="../TierIIData/2023_02/Rst_GC3_Muon_23-02-09v2"
#folder="../TierIIData/2023_02/Rst_FL_Muon_23-02-10_SiPM375"
folder="../TierIIData/2023_02/Rst_FL_Muon_23-02-09v2"

treeName1="EndOfEvent"
branchName1="fPrimaryZ"
treeName2="EndOfEvent"
branchName2="fphL"
treeName3="EndOfEvent"
branchName3="fphR"
plotName="EndOfEvent_fPrimaryZ_vs_fLC_box_NSGA_",
xMin=-29
xMax=29
yMin=0
yMax=6000
saveFolder="CSV/"
treeName3="EndOfEvent"
branchName4="fDdep",
# Create a figure and axes
fig, ax = plt.subplots(figsize=(15, 6))
ax.set_ylim(1000, 2000)

# Read data from root file

print("Reading X data")
with contextlib.redirect_stdout(io.StringIO()):
    data1 = readRootDataFromFolder(folder, treeName1, branchName1)
    data2 = readRootDataFromFolder(folder, treeName2, branchName2)
    data3 = readRootDataFromFolder(folder, treeName3, branchName3)
    edep = readRootDataFromFolder(folder, treeName1, "fEdep")

print(len (data1))
print(len(data2))
print(len(edep))

n = 40  # Number of ranges
range_size = xMax * 2 / n
import matplotlib.pyplot as plt

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
                if edep[j]>0:
                    values_in_range.append((data3[j] + data2[j]) / 2 / edep[j])
    data_in_ranges.append(values_in_range)
    # Calculate the box position and width
    box_position = (range_min + range_max) / 2*1000
    box_width = 0.9 * (range_max - range_min)*1000
    box_positions.append(box_position)
    box_widths.append(box_width)


# Set the y-axis limits
print(box_positions)

# Plot the box plot for each range
boxplot=ax.boxplot(data_in_ranges, positions=box_positions, widths=box_widths, showfliers=True,
          patch_artist=False,  # Set patch_artist to False
                     whiskerprops=dict(color='blue'), capprops=dict(color='blue'),
                     medianprops=dict(color='blue'), flierprops=dict(marker='o', markersize=5,
                                                                     markerfacecolor='black'), whis=1.5)



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
ax.set_xlabel('Length [mm]')
ax.set_ylabel('Light Collection [ph]')

# Customize the color of all boxes
for box in boxplot['boxes']:
    box.set_color('blue')

import matplotlib.ticker as mticker

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

print(x)
print(y)
print(x_filtered)
print(y_filtered)

# Add a parabolic fit to the mean values
x_fit = np.linspace(min(x_filtered), max(x_filtered), 100)
y_fit = np.polyval(np.polyfit(x_filtered, y_filtered, 2), x_fit)
ax.plot(x_fit, y_fit, color='red', linestyle='--')
print(x_fit)
print(y_fit)

# Save the figure as a PDF
output_filename = 'CSV/LC375_ROOT_LO.pdf'
plt.savefig(output_filename, format='pdf')
# Show the plot
plt.show()
print(box_means)
print(y_fit)