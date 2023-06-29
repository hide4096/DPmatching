import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

# CSVファイルのパス
csv_file = 'similarity_results.csv'

# CSVファイルを読み込む
df = pd.read_csv(csv_file, index_col=0)

# ヒートマップを描画
plt.figure(figsize=(10, 8))
sns.set(font_scale=1.2)  # フォントサイズを調整
sns.heatmap(df, cmap='BuGn', annot=True, fmt=".2f", cbar=False, vmin=df.min().min(), vmax=df.max().max(), linewidths=0.5, linecolor='lightgray')
plt.xlabel('File 2')
plt.ylabel('File 1')
plt.title('Similarity Heatmap')
plt.xticks(rotation=45, ha='right')
plt.yticks(rotation=0)
plt.tight_layout()
plt.show()
