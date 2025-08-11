import csv
import matplotlib.pyplot as plt
import numpy as np

with open('output.csv', 'r', encoding='utf-8') as file:
    csv_reader = csv.DictReader(file)
    rows = list(csv_reader)

smell_types = []
for row in rows:
    if row['smell_type'] not in smell_types:
        smell_types.append(row['smell_type'])

# one plot for each smell type
for smell_type in smell_types:
    file_names = []
    all_metrics = []  # stores dictionaries of metric_name: value
   
    count = 0
    for row in rows:
        if row['smell_type'] == smell_type and count < 5:
            # extract file name from path
            short_filename = row['file_name'].split('/')[-1]
            file_names.append(f"{short_filename}\n(line {row['line']})")
           
            metrics = {}
            if row['metric1_name'] and row['metric1_measured_value']:
                metrics[row['metric1_name']] = float(row['metric1_measured_value'])
            if row['metric2_name'] and row['metric2_measured_value']:
                metrics[row['metric2_name']] = float(row['metric2_measured_value'])
            if row['metric3_name'] and row['metric3_measured_value']:
                metrics[row['metric3_name']] = float(row['metric3_measured_value'])
           
            all_metrics.append(metrics)
            count += 1
   
    if not file_names:
        continue
   
    # get all unique metric names for this smell type
    metric_names = set()
    for metrics in all_metrics:
        metric_names.update(metrics.keys())
    metric_names = list(metric_names)
   
    # create grouped bar plot
    x_positions = np.arange(len(file_names))
    bar_width = 0.8 / len(metric_names)
    colors = ['steelblue', 'orange', 'green', 'red', 'purple']
   
    plt.figure(figsize=(14, 8))
   
    # create bars for each metric
    for i, metric_name in enumerate(metric_names):
        values = []
        for metrics in all_metrics:
            values.append(metrics.get(metric_name, 0))  # use 0 if metric not present
       
        bars = plt.bar(x_positions + i * bar_width - (len(metric_names)-1) * bar_width/2,
                      values, bar_width, label=metric_name, color=colors[i % len(colors)], alpha=0.8)
       
        # add value labels on top of bars
        for bar, value in zip(bars, values):
            if value > 0:  # only show label if value exists
                plt.text(bar.get_x() + bar.get_width()/2, bar.get_height() + max(values)*0.01,
                        f'{value:.2f}' if value < 1 else str(int(value)),
                        ha='center', va='bottom', fontsize=9)
   
    #plt.title(f"Worst 5 {smell_type.replace('_', ' ').title()} Smells")
    plt.xlabel("Files")
    plt.ylabel("Metric Values")
    plt.xticks(x_positions, file_names, rotation=45, ha='right')
    plt.legend()
    plt.tight_layout()
    plt.show()