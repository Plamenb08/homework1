# Мини Библиотечна Система (Library System)

## Описание

Този проект представлява малка библиотечна система, реализирана на C++ с обектно-ориентиран подход. Целта е да се демонстрират принципите на капсулация, Rule of 5, конструктори и деструктори, статични членове, константни методи и валидация на данни.

### Класове

1. **Author**
   - Членове: `string name`, `int birthYear`
   - Конструктори: по подразбиране и параметризиран
   - Методи: `to_string() const`, getters, `setBirthYear(int)` с валидация
   - Добри практики: `const` методи, `explicit` конструктори

2. **Book**
   - Членове: `string title`, `Author author`, `int year`, `double price`, `string isbn`
   - Статичен член: `static int totalBooks` (брои живите инстанции)
   - Конструктори: по подразбиране, параметризиран, копиращ и преместващ
   - Rule of 5: копиращ и преместващ оператор, деструктор
   - Методи: getters/setters с валидация (цена ≥ 0, годината в разумен диапазон), `to_string() const`, `static getTotalBooks()`
   
3. **Member**
   - Членове: `string name`, `string memberId`, `int yearJoined`
   - Конструктори: по подразбиране и параметризиран
   - Методи: валидации (например memberId да не е празен), `to_string() const`
   
4. **Loan**
   - Членове: `string isbn`, `string memberId`, `string startDate`, `string dueDate`, `bool returned`
   - Методи: `markReturned()`, `isOverdue(const string& today) const`, `to_string() const`
   - Конструктор: параметризиран с валидация (dueDate >= startDate)
   
5. **Library**
   - Членове: `vector<Book> books`, `vector<Member> members`, `vector<Loan> loans`
   - Методи:
     - `addBook(const Book& b)`
     - `addMember(const Member& m)`
     - `hasBook(const string& isbn) const`
     - `isBookAvailable(const string& isbn) const`
     - `loanBook(const string& isbn, const string& memberId, const string& start, const string& due)`
     - `returnBook(const string& isbn, const string& memberId)`
     - `findByAuthor(const string& authorName) const`
     - `to_string() const` (обобщена информация: брой книги/членове/активни заеми)
   - Добри практики: максимално `const`-коректност, ясни инварианти, без „голи“ указатели
