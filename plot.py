# this python script plots the total occurence of each smell type
# per file as a bar plot
# ! only the top 10 files

#TODO: add more analysis functionality:
# e.g. severity calculation based on difference between
# threshold and actual value
# -> more plots

import pandas as pd
import matplotlib.pyplot as plt
import os

df = pd.read_csv("output.csv")

# strips path from file name
df['file_name'] = df['file_name'].apply(os.path.basename)

# transform into plotable dataframe
grouped = df.groupby(['file_name', 'smell_type']).size().reset_index(name='count')
pivot_df = grouped.pivot(index='file_name', columns='smell_type', values='count')

# replace NaN with 0
pivot_df = pivot_df.fillna(0)

# only top 10 files with most smell
pivot_df['total'] = pivot_df.sum(axis=1)
top = pivot_df.sort_values('total', ascending=False).head(10).drop(columns='total')


# plot
ax = top.plot(kind='bar', stacked=True)
plt.title("Smell Counts per File")
ax.set_xticklabels(ax.get_xticklabels(), rotation=0)
plt.show()
