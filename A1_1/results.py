import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from pathlib import Path
import os

current_dir = Path.cwd()
print(f"Current directory: {current_dir}")
print()

possible_paths = [
    current_dir / 'benchmark_results.csv',                   
    current_dir / 'results' / 'benchmark_results.csv',
    current_dir.parent / 'results' / 'benchmark_results.csv',
    current_dir.parent.parent / 'results' / 'benchmark_results.csv',
]


csv_path = None
for path in possible_paths:
    if path.exists():
        csv_path = path



df = pd.read_csv(csv_path)

figures_dir = csv_path.parent / 'figures'
figures_dir.mkdir(parents=True, exist_ok=True)

algorithms = {
    'std_quicksort': {'label': 'Standard Quicksort', 'color': '#FF6B6B', 'marker': 'o', 'line': '-'},
    'std_mergesort': {'label': 'Standard Mergesort', 'color': '#4ECDC4', 'marker': 's', 'line': '-'},
    'string_quicksort': {'label': 'String Quicksort (3-way)', 'color': '#45B7D1', 'marker': '^', 'line': '--'},
    'string_mergesort': {'label': 'String Mergesort (LCP)', 'color': '#96CEB4', 'marker': 'D', 'line': '--'},
    'msd_radix': {'label': 'MSD Radix Sort', 'color': '#FFEAA7', 'marker': 'v', 'line': '-.'},
    'msd_radix_cutoff': {'label': 'MSD Radix + Cutoff', 'color': '#DDA0DD', 'marker': 'p', 'line': '-.'}
}

array_types = df['array_type'].unique()
titles = {
    'random': 'Random Arrays',
    'reverse_sorted': 'Reverse Sorted Arrays',
    'nearly_sorted': 'Nearly Sorted Arrays',
    'common_prefix': 'Arrays with Common Prefix'
}

print("GENERATING FIGURES")

fig1, axes1 = plt.subplots(2, 2, figsize=(15, 11))
axes1 = axes1.flatten()

for idx, array_type in enumerate(array_types):
    ax = axes1[idx]
    subset = df[df['array_type'] == array_type]
    
    for algo, props in algorithms.items():
        algo_data = subset[subset['algorithm'] == algo]
        if not algo_data.empty:
            ax.plot(algo_data['size'], algo_data['time_ms'], 
                   marker=props['marker'],
                   linestyle=props['line'],
                   linewidth=2, markersize=8,
                   label=props['label'],
                   color=props['color'],
                   alpha=0.85)
    
    ax.set_xlabel('Array Size', fontsize=12, fontweight='bold')
    ax.set_ylabel('Time (milliseconds)', fontsize=12, fontweight='bold')
    ax.set_title(titles.get(array_type, array_type), fontsize=14, fontweight='bold')
    ax.legend(loc='upper left', fontsize=9, framealpha=0.95)
    ax.grid(True, alpha=0.3)
    ax.set_xlim(0, 3100)

plt.suptitle('Execution Time Comparison of String Sorting Algorithms', 
             fontsize=16, fontweight='bold', y=1.02)
plt.tight_layout()
plt.savefig(figures_dir / '1_time_comparison.png', dpi=300, bbox_inches='tight')
plt.close()
print("Figure 1: 1_time_comparison.png")

fig2, axes2 = plt.subplots(2, 2, figsize=(15, 11))
axes2 = axes2.flatten()

for idx, array_type in enumerate(array_types):
    ax = axes2[idx]
    subset = df[df['array_type'] == array_type]
    
    for algo, props in algorithms.items():
        algo_data = subset[subset['algorithm'] == algo]
        if not algo_data.empty:
            ax.plot(algo_data['size'], algo_data['comparisons'], 
                   marker=props['marker'],
                   linestyle=props['line'],
                   linewidth=2, markersize=8,
                   label=props['label'],
                   color=props['color'],
                   alpha=0.85)
    
    ax.set_xlabel('Array Size', fontsize=12, fontweight='bold')
    ax.set_ylabel('Character Comparisons', fontsize=12, fontweight='bold')
    ax.set_title(titles.get(array_type, array_type), fontsize=14, fontweight='bold')
    ax.legend(loc='upper left', fontsize=9, framealpha=0.95)
    ax.grid(True, alpha=0.3)
    ax.set_xlim(0, 3100)
    
    if array_type == 'reverse_sorted':
        ax.set_yscale('log')
        ax.set_ylabel('Character Comparisons (log scale)', fontsize=12, fontweight='bold')

plt.suptitle('Character Comparisons in String Sorting', 
             fontsize=16, fontweight='bold', y=1.02)
plt.tight_layout()
plt.savefig(figures_dir / '2_comparisons.png', dpi=300, bbox_inches='tight')
plt.close()
print("Figure 2: 2_comparisons.png")


fig3, axes3 = plt.subplots(2, 2, figsize=(15, 11))
axes3 = axes3.flatten()

for idx, array_type in enumerate(array_types):
    ax = axes3[idx]
    subset = df[df['array_type'] == array_type]
    
    baseline = subset[subset['algorithm'] == 'std_quicksort'][['size', 'time_ms']]
    baseline = baseline.set_index('size')['time_ms']
    
    for algo, props in algorithms.items():
        if algo == 'std_quicksort':
            continue
        algo_data = subset[subset['algorithm'] == algo]
        if not algo_data.empty:
            algo_data = algo_data.set_index('size')
            speedup = baseline[algo_data.index] / algo_data['time_ms']
            ax.plot(speedup.index, speedup.values,
                   marker=props['marker'],
                   linestyle=props['line'],
                   linewidth=2, markersize=8,
                   label=props['label'],
                   color=props['color'],
                   alpha=0.85)
    
    ax.axhline(y=1.0, color='red', linestyle='--', linewidth=2, alpha=0.7, label='Baseline (1x)')
    ax.set_xlabel('Array Size', fontsize=12, fontweight='bold')
    ax.set_ylabel('Speedup Factor', fontsize=12, fontweight='bold')
    ax.set_title(f'Speedup vs Standard Quicksort - {titles.get(array_type, array_type)}', 
                fontsize=12, fontweight='bold')
    ax.legend(loc='best', fontsize=9, framealpha=0.95)
    ax.grid(True, alpha=0.3)
    ax.set_xlim(0, 3100)

plt.suptitle('Performance Improvement of String-Optimized Algorithms', 
             fontsize=16, fontweight='bold', y=1.02)
plt.tight_layout()
plt.savefig(figures_dir / '3_speedup.png', dpi=300, bbox_inches='tight')
plt.close()
print("Figure 3: 3_speedup.png")

fig4, axes4 = plt.subplots(1, 2, figsize=(15, 6))

largest_size = df[df['size'] == 3000]


ax1 = axes4[0]
x_pos = np.arange(len(array_types))
width = 0.12

for i, (algo, props) in enumerate(algorithms.items()):
    times = []
    for atype in array_types:
        val = largest_size[(largest_size['array_type'] == atype) & 
                           (largest_size['algorithm'] == algo)]['time_ms'].values
        times.append(val[0] if len(val) > 0 else 0)
    
    offset = (i - len(algorithms)/2 + 0.5) * width
    ax1.bar(x_pos + offset, times, width, 
            label=props['label'], color=props['color'],
            edgecolor='black', linewidth=0.5, alpha=0.85)

ax1.set_xlabel('Array Type', fontsize=12, fontweight='bold')
ax1.set_ylabel('Time (ms)', fontsize=12, fontweight='bold')
ax1.set_title('Execution Time for n=3000', fontsize=14, fontweight='bold')
ax1.set_xticks(x_pos)
ax1.set_xticklabels([t.replace('_', ' ').title() for t in array_types], fontsize=10)
ax1.legend(loc='upper left', fontsize=8, ncol=2, framealpha=0.95)
ax1.grid(True, alpha=0.3, axis='y')


ax2 = axes4[1]
algo_efficiency = {}

for algo, props in algorithms.items():
    if algo == 'std_quicksort':
        continue
    reductions = []
    for atype in array_types:
        algo_val = largest_size[(largest_size['array_type'] == atype) & 
                                (largest_size['algorithm'] == algo)]['comparisons'].values
        std_val = largest_size[(largest_size['array_type'] == atype) & 
                               (largest_size['algorithm'] == 'std_quicksort')]['comparisons'].values
        if len(algo_val) > 0 and len(std_val) > 0 and std_val[0] > 0:
            reduction = (1 - algo_val[0] / std_val[0]) * 100
            reductions.append(reduction)
    if reductions:
        algo_efficiency[props['label']] = np.mean(reductions)

algo_names = list(algo_efficiency.keys())
reduction_values = list(algo_efficiency.values())
colors = [algorithms[a]['color'] for a in algorithms if algorithms[a]['label'] in algo_names]

bars = ax2.barh(algo_names, reduction_values, color=colors, edgecolor='black', linewidth=0.5)
ax2.set_xlabel('Average Reduction in Comparisons (%)', fontsize=12, fontweight='bold')
ax2.set_title('Efficiency vs Standard Quicksort (n=3000)', fontsize=14, fontweight='bold')
ax2.grid(True, alpha=0.3, axis='x')

for bar, val in zip(bars, reduction_values):
    ax2.text(val + 1, bar.get_y() + bar.get_height()/2, 
             f'{val:.1f}%', va='center', fontsize=10, fontweight='bold')

plt.tight_layout()
plt.savefig(figures_dir / '4_summary_comparison.png', dpi=300, bbox_inches='tight')
plt.close()
print("Figure 4: 4_summary_comparison.png")


print("\n")
print("PERFORMANCE SUMMARY (n=3000)")

for array_type in array_types:
    print(f"\n{titles.get(array_type, array_type)}:")
    print("")
    
    subset = largest_size[largest_size['array_type'] == array_type]
    baseline_time = subset[subset['algorithm'] == 'std_quicksort']['time_ms'].values[0]
    baseline_cmp = subset[subset['algorithm'] == 'std_quicksort']['comparisons'].values[0]
    
    print(f"{'Algorithm':<30} {'Time (ms)':<12} {'Comparisons':<15} {'Speedup':<10}")
    print("")
    
    for algo, props in algorithms.items():
        algo_data = subset[subset['algorithm'] == algo]
        if not algo_data.empty:
            time = algo_data['time_ms'].values[0]
            comparisons = algo_data['comparisons'].values[0]
            speedup = baseline_time / time
            
            print(f"{props['label']:<30} {time:<12.2f} {comparisons:<15,} {speedup:<10.2f}x")

