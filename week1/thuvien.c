#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Viết chương trình quản lý sách, trong đó sách có các thuộc tính sau:
1. Tên sách
2. Tác giả
Lưu ý: Không dùng mảng động

Giao diện console có các lựa chọn sau:
- Thêm sách
- Xóa sách
*/

// const at array declare is not permitted in C
#define author_len 21 // one for string terminator
#define title_len 51 // one for string terminator
#define capacity 10

struct Book {
  char author[author_len];
  char title[title_len];
  char inUse;
};

typedef struct Book Book;

enum CHOICES {ADDBOOK = 1, DELETEBOOK, EXIT, VIEWBOOK};

Book library[capacity];
int num_book = 0;

void addBook();
void deleteBook();

int main(int argc, char** argv) {
  int choice = 0;
  int s = 0;
  while (1) {
    // program main loop
    printf("LIBRARY\n");
    printf("1. add a book\n");
    printf("2. delete a book\n");
	printf("3. exit\n");
    printf(">> ");
    s = scanf("%d", &choice);
    if (s != 1) {
      printf("Please input again!");
      getchar();
      continue;
    }
    switch (choice) {
      case ADDBOOK:
        addBook();
        break;
      case DELETEBOOK:
        deleteBook();
        break;
	  case EXIT:
		return 0;
      default:
        break;
    }
  }
  return 0;
}

void addBook() {
  if (num_book >= capacity) { // watch for overflow
    // het cho dung book
    // 1. Tang capacity, cap phat lai thu vien
    // 2. Khong cho them
    printf("The library is full, no more book can be added, delete some book\n");
    getchar();
    return;
  }
  char title[title_len];
  char author[author_len];
  printf("Enter book title: ");
  scanf("%50s", title);
  printf("Enter author's name: ");
  scanf("%20s", author);

  printf("Add book %s by %s? (y/n) ", title, author);
  char ok;
  while ((ok = getchar()) == '\n' || ok == '\r' || ok == EOF);
  if (ok != 'y') {
    printf("The operation has been cancled\n");
    printf("Press enter to continue");
    getchar();
    return;
  }

  int idx;
  for (idx = 0; idx < capacity; idx++)
    if (library[idx].inUse == 0)
      break;

  Book* newBook = &library[idx];
  strcpy(newBook->author, author);
  strcpy(newBook->title, title);
  newBook->inUse = 1;

  printf("The book is added with id: %d\n", idx);
  num_book++;
}

void deleteBook() {
  if (num_book <= 0)
    return;
  int idx;
  printf("Enter book Id that needs deletion: ");
  scanf("%d", &idx);

  if (idx < 0 || idx >= capacity) {
    printf("Id is not appropriate\n");
    return;
  }

  Book* toBeDeletedBook = &library[idx];

  if (toBeDeletedBook->inUse == 0) {
    printf("The book is not present\n");
    return;
  }

  printf("Delete book %s by %s? (y/n)", toBeDeletedBook->title, toBeDeletedBook->author);
  char ok = 0;
  while ((ok = getchar()) == '\n' || ok == '\r' || ok == EOF);
  if (ok != 'y') {
    printf("The operation has been cancled\n");
    printf("Press enter to continue");
    getchar();
    return;
  }

  toBeDeletedBook->inUse = 0;
  strcpy(toBeDeletedBook->author, "");
  strcpy(toBeDeletedBook->title, "");
  printf("Successfully delete book with Id: %d\n", idx);
  num_book--;
}
