import os
import subprocess
import sys

def calculate_similarity(file1, file2):
    arguments = [file1,file2]
    #print(arguments)

    process = subprocess.run([executable_path] + arguments, capture_output=True, text=True)
    similarity = float(process.stdout)
    return similarity

def correct_rate(dir1, dir2):
    raw_paths1 = [os.path.join(dir1, f) for f in os.listdir(dir1) if os.path.isfile(os.path.join(dir1, f))]
    raw_paths2 = [os.path.join(dir2, f) for f in os.listdir(dir2) if os.path.isfile(os.path.join(dir2, f))]
    file_paths1 = sorted(raw_paths1)
    file_paths2 = sorted(raw_paths2)

    incorrect = 0
    q = 1
    a = 1

    for path1 in file_paths1:
        min = 10.
        a = 1
        for path2 in file_paths2:
            similarity = calculate_similarity(path1, path2)
            if q == a and similarity > min:
                print(f'{q} Incorrect')
                incorrect+=1
                break
            if q<a and similarity < min:
                print(f'{q} Incorrect')
                incorrect+=1
                break
            if similarity < min:
                min = similarity
            a+=1
        q+=1
        #print(f'\r{round(q/len(file_paths1)*100.0)}%', end='')
    
    print()
    return (a-incorrect)/q

args = sys.argv

executable_path = args[1]
dir1_path = args[2]
dir2_path = args[3]

rate = correct_rate(dir1_path, dir2_path)
print(f"CorrectRate: {round(rate*100.0)}%")
