#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

class Author {
private:
    std::string name;
    int birthYear;
public:
    Author(const std::string& name = "Unknown", int birthYear = 1900)
        : name(name) {
        setBirthYear(birthYear);
    }
    const std::string& getName() const { return name; }
    int getBirthYear() const { return birthYear; }
    void setBirthYear(int y) {
        if (y < 1850 || y > 2025)
            throw std::invalid_argument("Invalid birth year");
        birthYear = y;
    }
    std::string to_string() const {
        return "Author: " + name + " (" + std::to_string(birthYear) + ")";
    }
};

class Book {
private:
    std::string title;
    Author author;
    int year;
    double price;
    std::string isbn;
public:
    static int totalBooks;
    Book()
        : title("Untitled"), author(), year(2000), price(0.0), isbn("0000") {
        totalBooks++;
    }
    Book(const std::string& title, const Author& author,
         int year, double price, const std::string& isbn)
        : title(title), author(author), year(year), price(price), isbn(isbn) {
        setYear(year);
        setPrice(price);
        totalBooks++;
    }
    Book(const Book&) = default;
    Book(Book&&) noexcept = default;
    Book& operator=(const Book&) = default;
    Book& operator=(Book&&) noexcept = default;
    ~Book() = default;
    const std::string& getTitle() const { return title; }
    const Author& getAuthor() const { return author; }
    int getYear() const { return year; }
    double getPrice() const { return price; }
    const std::string& getISBN() const { return isbn; }
    void setYear(int y) {
        if (y < 1500 || y > 2025) throw std::invalid_argument("Invalid year");
        year = y;
    }
    void setPrice(double p) {
        if (p < 0) throw std::invalid_argument("Price cannot be negative");
        price = p;
    }
    std::string to_string() const {
        return title + " (" + isbn + ")";
    }
    static int getTotalBooks() { return totalBooks; }
};

int Book::totalBooks = 0;

class Member {
private:
    std::string name;
    std::string memberId;
    int yearJoined;
public:
    Member(const std::string& name = "Unknown",
           const std::string& id = "none",
           int yearJoined = 2000)
        : name(name), memberId(id), yearJoined(yearJoined) {
        if (id.empty()) throw std::invalid_argument("memberId cannot be empty");
    }
    const std::string& getName() const { return name; }
    const std::string& getId() const { return memberId; }
    std::string to_string() const {
        return "Member: " + name + " (" + memberId + ")";
    }
};

class Loan {
private:
    std::string isbn;
    std::string memberId;
    std::string startDate;
    std::string dueDate;
    bool returned;
public:
    Loan(const std::string& isbn, const std::string& memberId,
         const std::string& start, const std::string& due)
        : isbn(isbn), memberId(memberId), startDate(start), dueDate(due), returned(false) {
        if (due < start)
            throw std::invalid_argument("dueDate cannot be earlier than startDate");
    }
    void markReturned() { returned = true; }
    bool isReturned() const { return returned; }
    bool isOverdue(const std::string& today) const {
        return (!returned && today > dueDate);
    }
    const std::string& getISBN() const { return isbn; }
    const std::string& getMemberId() const { return memberId; }
    std::string to_string() const {
        return isbn + " loaned to " + memberId + (returned ? " [Returned]" : " [Active]");
    }
};

class Library {
private:
    std::vector<Book> books;
    std::vector<Member> members;
    std::vector<Loan> loans;
public:
    void addBook(const Book& b) { books.push_back(b); }
    void addMember(const Member& m) { members.push_back(m); }
    bool hasBook(const std::string& isbn) const {
        for (const auto& b : books)
            if (b.getISBN() == isbn) return true;
        return false;
    }
    bool isBookAvailable(const std::string& isbn) const {
        for (const auto& l : loans)
            if (l.getISBN() == isbn && !l.isReturned()) return false;
        return true;
    }
    bool loanBook(const std::string& isbn, const std::string& memberId,
                  const std::string& start, const std::string& due) {
        if (!hasBook(isbn) || !isBookAvailable(isbn))
            return false;
        loans.emplace_back(isbn, memberId, start, due);
        return true;
    }
    bool returnBook(const std::string& isbn, const std::string& memberId) {
        for (auto& l : loans)
            if (l.getISBN() == isbn && l.getMemberId() == memberId && !l.isReturned()) {
                l.markReturned();
                return true;
            }
        return false;
    }
    std::vector<Book> findByAuthor(const std::string& name) const {
        std::vector<Book> result;
        for (const auto& b : books)
            if (b.getAuthor().getName() == name)
                result.push_back(b);
        return result;
    }
    std::string to_string() const {
        int active = 0;
        for (const auto& l : loans) if (!l.isReturned()) active++;
        return "Library: " + std::to_string(books.size()) + " books, " +
               std::to_string(members.size()) + " members, " +
               std::to_string(active) + " active loans";
    }
    void printLoans() const {
        for (const auto& l : loans)
            std::cout << l.to_string() << "\n";
    }
};

int main() {
    Library lib;
    Author rowling("J. K. Rowling", 1965);
    Author tolkien("J. R. R. Tolkien", 1892);
    Book hp1("Harry Potter and the Philosopher's Stone", rowling, 1997, 20.0, "HP001");
    Book hp2("Harry Potter and the Chamber of Secrets", rowling, 1998, 22.0, "HP002");
    Book lotr("The Lord of the Rings", tolkien, 1954, 30.0, "LOTR001");
    lib.addBook(hp1);
    lib.addBook(hp2);
    lib.addBook(lotr);
    Member m1("Ivan Ivanov", "M001", 2020);
    Member m2("Maria Petrova", "M002", 2021);
    lib.addMember(m1);
    lib.addMember(m2);
    std::cout << lib.to_string() << "\n\n";
    lib.loanBook("HP001", "M001", "2025-12-01", "2025-12-15");
    lib.loanBook("LOTR001", "M002", "2025-12-02", "2025-12-20");
    std::cout << "After loans:\n" << lib.to_string() << "\n";
    lib.printLoans();
    std::cout << "\n";
    if (!lib.loanBook("HP001", "M002", "2025-12-05", "2025-12-20"))
        std::cout << "Book HP001 is already loaned. Cannot loan to M002.\n\n";
    lib.returnBook("HP001", "M001");
    std::cout << "After returning HP001:\n" << lib.to_string() << "\n";
    lib.printLoans();
    std::cout << "\n";
    std::vector<Book> rowlingBooks = lib.findByAuthor("J. K. Rowling");
    std::cout << "Books by J. K. Rowling:\n";
    for (const auto& b : rowlingBooks)
        std::cout << "- " << b.to_string() << "\n";
    return 0;
}
