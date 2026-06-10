import os
import sys
import ebooklib
from ebooklib import epub
from bs4 import BeautifulSoup
import PyPDF2
from markdown import markdown

def epub_to_txt(epub_path, txt_path):
    """Converts an EPUB file to a text file."""
    book = epub.read_epub(epub_path)
    with open(txt_path, 'w', encoding='utf-8') as txt_file:
        for item in book.get_items():
            if item.get_type() == ebooklib.ITEM_DOCUMENT:
                soup = BeautifulSoup(item.get_content(), 'html.parser')
                txt_file.write(soup.get_text() + '

')
    print(f"Successfully converted {epub_path} to {txt_path}")

def pdf_to_txt(pdf_path, txt_path):
    """Converts a PDF file to a text file."""
    with open(pdf_path, 'rb') as pdf_file:
        pdf_reader = PyPDF2.PdfReader(pdf_file)
        with open(txt_path, 'w', encoding='utf-8') as txt_file:
            for page_num in range(len(pdf_reader.pages)):
                page = pdf_reader.pages[page_num]
                txt_file.write(page.extract_text() + '

')
    print(f"Successfully converted {pdf_path} to {txt_path}")

def md_to_txt(md_path, txt_path):
    """Converts a Markdown file to a text file."""
    with open(md_path, 'r', encoding='utf-8') as md_file:
        html = markdown(md_file.read())
        soup = BeautifulSoup(html, 'html.parser')
        with open(txt_path, 'w', encoding='utf-8') as txt_file:
            txt_file.write(soup.get_text())
    print(f"Successfully converted {md_path} to {txt_path}")

def main():
    if len(sys.argv) != 2:
        print("Usage: python convert_script.py <file_path>")
        sys.exit(1)

    input_path = sys.argv[1]
    if not os.path.exists(input_path):
        print(f"Error: File not found at {input_path}")
        sys.exit(1)

    file_name, file_ext = os.path.splitext(input_path)
    output_path = file_name + '.txt'

    if file_ext == '.epub':
        epub_to_txt(input_path, output_path)
    elif file_ext == '.pdf':
        pdf_to_txt(input_path, output_path)
    elif file_ext in ['.md', '.markdown']:
        md_to_txt(input_path, output_path)
    else:
        print(f"Error: Unsupported file format {file_ext}")
        print("Supported formats: .epub, .pdf, .md, .markdown")
        sys.exit(1)

if __name__ == '__main__':
    main()
