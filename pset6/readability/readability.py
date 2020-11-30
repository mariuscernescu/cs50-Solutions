from cs50 import get_string

def main():
    text = get_string("Text: ") # Take an input text
    lettersCount = countLetters(text) # Count letters
    wordCount = countWords(text) # Count words
    sentCount = countSentences(text) # Count sentences
    cIndex = colIndex(lettersCount, wordCount, sentCount) # Calculate Coleman-Liau index
    
    # Print results
    if cIndex < 1:
        print("Before Grade 1")
    elif cIndex > 16:
        print("Grade 16+")
    else:
        print(f"Grade {cIndex}")

def countLetters(text):
    count = 0
    for c in text:
        if c.isalpha():
            count += 1
    return count

def countWords(text): # Counting white spaces
    counter = 1;
    for i in text:
        if i == " ":
            counter += 1
    return counter

def countSentences(text): # Counting if ".", "!", "?" are present
    count = 0;
    sentStop = [".", "!", "?"]
    for c in text:
        if c in sentStop:
            count += 1
    return count

# Calculate the Coleman-Liau index
def colIndex(l, w, s):
    return round(0.0588 * ((l * 100) / w) - 0.296 * ((s * 100) / w) - 15.8)

main()