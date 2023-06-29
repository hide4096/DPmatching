#!/bin/bash

# 第1引数と第2引数にディレクトリのパスを指定する
dir1="$1"
dir2="$2"

# ディレクトリ内のテキストファイルのパスを取得する関数
get_text_files() {
    local dir_path="$1"
    find "$dir_path" -type f -name "*.txt" | sort
}

# 2つのファイルを比較して類似度を計算する関数
calculate_similarity() {
    local file1="$1"
    local file2="$2"
    # 類似度を計算するプログラムを実行し、結果を返す
    similarity=$(./a.out "$file1" "$file2")
    echo "$similarity"
}

# CSVファイルに結果を書き込む関数
write_to_csv() {
    local output_file="$1"
    local data="$2"
    echo "$data" >> "$output_file"
}

# 結果を保存するCSVファイルのパス
output_file="similarity_results.csv"
echo > "$output_file" # ファイルを空にする

# ディレクトリ内のテキストファイルのパスを取得
files1=$(get_text_files "$dir1")
files2=$(get_text_files "$dir2")

# CSVファイルのヘッダを書き込む
write_to_csv "$output_file" ",$(basename -a $files2 | sed 's/.txt//g' | paste -sd ',')" # ファイル名のヘッダ行

# ファイルの総数を計算
total_files=$(echo "$files1" | wc -l)
total_files=$((total_files * $(echo "$files2" | wc -l)))

# 現在の処理中のファイル番号と進捗を表示しながら類似度を計算し、結果をCSVファイルに書き込む
current_file=0
for file1 in $files1; do
    row=$(basename "$file1" | sed 's/.txt//g') # 行のヘッダ
    row_data="$row"
    for file2 in $files2; do
        current_file=$((current_file + 1))
        similarity=$(calculate_similarity "$file1" "$file2")
        row_data="$row_data,$similarity"
        progress=$((current_file * 100 / total_files))
        echo -ne "進捗: $progress% ($current_file/$total_files)\r"
    done
    write_to_csv "$output_file" "$row_data"
done

echo "類似度の結果を $output_file に出力しました。"
