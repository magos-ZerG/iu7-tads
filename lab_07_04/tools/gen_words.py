#!/usr/bin/env python3
import argparse
import random
import string
import sys
import os

def gen_random_word(length):
    return ''.join(random.choices(string.ascii_lowercase, k=length))

def gen_words_set(wordsnum, wordsize):
    words_set = set()
    while len(words_set) < wordsnum:
        words_set.add(gen_random_word(wordsize))
    return list(words_set)

def gen_balanced(sorted_words):
    if not sorted_words:
        return []
    mid = len(sorted_words) // 2
    return [sorted_words[mid]] + gen_balanced(sorted_words[:mid]) + gen_balanced(sorted_words[mid+1:])

def main():
    parser = argparse.ArgumentParser(description="Generate BST")
    parser.add_argument("--wordsnum", type=int, required=True, help="Number of words")
    parser.add_argument("--wordsize", type=int, required=True, help="Size of each word")
    args = parser.parse_args()

    wordsnum = args.wordsnum
    wordsize = args.wordsize

    if wordsnum <= 0 or wordsize <= 0:
        print("Wordsnum and wordsize must be positive")
        sys.exit(1)

    words = gen_words_set(wordsnum, wordsize)

    folder_path = "./out"
    if not os.path.exists(folder_path):
        os.makedirs(folder_path)

    random.shuffle(words)
    with open(f"./out/test_{wordsnum}_random.txt", "w") as f:
        f.write(" ".join(words))
        f.write("\n")

    balanced_tree = gen_balanced(sorted(words))
    with open(f"./out/test_{wordsnum}_balanced.txt", "w") as f:
        f.write(" ".join(balanced_tree))
        f.write("\n")

    degenerated_tree = sorted(words)
    with open(f"./out/test_{wordsnum}_degenerated.txt", "w") as f:
        f.write(" ".join(degenerated_tree))
        f.write("\n")

if __name__ == "__main__":
    main()
