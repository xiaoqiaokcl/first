#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>//输出格式控制
#include <algorithm>
#include <sstream>
#include <limits>
#include<cctype>

using namespace std;

// 日期类 工具
class Date {

private:
    int year;
    int month;
    int day;

public:
    //无参构造函数，默认当前时间
    Date() {
        //获取时间戳
        time_t now = time(0);
        // 转换成时间体。使用线程安全的 localtime_s 在 MSVC 上，其他平台回退到 localtime。
#ifdef _MSC_VER
        struct tm tmStruct;
        if (localtime_s(&tmStruct, &now) == 0) {
            year = 1900 + tmStruct.tm_year;
            month = 1 + tmStruct.tm_mon;
            day = tmStruct.tm_mday;
        }
        else {
            year = 1970; month = 1; day = 1;
        }
#else
        struct tm* tmPtr = localtime(&now);
        if (tmPtr) {
            year = 1900 + tmPtr->tm_year;
            month = 1 + tmPtr->tm_mon;
            day = tmPtr->tm_mday;
        }
        else {
            year = 1970; month = 1; day = 1;
        }
#endif
    }

    //传参构造函数
    Date(int y, int m, int d) : year(y), month(m), day(d) {}

    // 日期验证
    bool isValid() const {

        if (year < 1900 || year > 2100)
        {
            return false;
        }

        if (month < 1 || month > 12)
        {
            return false;
        }

        //月份天数
        int maxDay[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
        //闰年判断
        if (month == 2) {
            if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
                maxDay[1] = 29;
            }
        }

        if (day >= 1 && day <= maxDay[month - 1]) {
            return true;
        }
        else return false;
    }

    // 日期转字符串
    string toString() const {
        stringstream ss;
        ss << setfill('0') << setw(4) << year << "-"
            << setw(2) << month << "-" << setw(2) << day;
        string result=ss.str();
        return result;
    }

    // 字符串转日期：先创建一个空的Date类对象，传入
    static bool fromString(const string& str,Date&outdate) {
        if (str.empty()) {
            cout << "日期不能为空" << endl;
            return false;
        }
        int y,m,d;
        char dash=0;
        stringstream ss;
        ss << str;
        for (int i = 0; i < str.length();i++  ){
            if (isdigit(str[i]))continue;
            if (dash == 0) {
                switch (str[i]) {
                case '-':
                    dash = '-'; break;
                case ' ':
                    dash = ' '; break;
                case '/':
                    dash = '/'; break;
                case '.':
                    dash = '.'; break;
                default:
                    cout << "日期字符串输入形式不支持" << endl;
                    return false;
                }
            }
            else if(str[i]!=dash) {
                cout << "只支持统一的日期分隔符" << endl;
                return false;
            }
        }
        if (dash == 0) {
            cout << "错误：日期字符串无有效分隔符" << endl;
            return false;
        }
        ss >> y >> dash >> m >> dash >> d;
        Date temp(y, m, d);
        if (!temp.isValid())return false;
        outdate = temp;
        return true;
    }

    // 重载比较运算符>、<、>=、<=。
    bool operator<(const Date& other) const {
        bool re = 0;
        if (year != other.year) { 
            re= (year < other.year);
            return re;
        }
        else if (month != other.month) {
            re = (month < other.month);
            return re;
        }
        else { 
            re = (day < other.day);
            return re; 
        }
    }

    bool operator>(const Date& other) const {
        bool re = 0;
        if (year != other.year) {
            re = (year > other.year);
            return re;
        }
        else if (month != other.month) {
            re = (month > other.month);
            return re;
        }
        else {
            re = (day > other.day);
            return re;
        }
    }

    bool operator<=(const Date& other) const {
        bool re = 0;
        if (year != other.year) {
            re = (year <= other.year);
            return re;
        }
        else if (month != other.month) {
            re = (month <= other.month);
            return re;
        }
        else {
            re = (day <= other.day);
            return re;
        }
    }

    bool operator>=(const Date& other) const {
        bool re = 0;
        if (year != other.year) {
            re = (year >= other.year);
            return re;
        }
        else if (month != other.month) {
            re = (month >= other.month);
            return re;
        }
        else {
            re = (day >= other.day);
            return re;
        }
    }

    // 获取年/月/日（供统计使用）
    int getYear() const {
        int re = year;
        return re; 
    }
    int getMonth() const { 
        int re = month;
        return re; 
    }
    int getDay() const {
        int re=day;
        return re; 
    }
};

// 财务记录基类
class FinanceRecord {

protected:
    string id;         // 唯一记录ID
    Date date;         // 交易日期
    double amount;     // 金额
    string category;    // 类别
    string note;        // 备注
    string payment;     // 支付方式（现金/微信/支付宝/银行卡）

public:
    FinanceRecord() : amount(0.0) {}
    FinanceRecord(const string& i, const Date& d, double a, const string& c, const string& n, const string& p)
        : id(i), date(d), amount(a), category(c), note(n), payment(p) {}

    // 纯虚函数：确保子类实现特定功能
    virtual string getType() const = 0;
    virtual string toFileString() const = 0;

    //  getter方法
    string getID() const { 
        return id;}
    Date getDate() const { 
        return date;}
    double getAmount() const {
        return amount;}
    string getCategory() const {
        return category; }
    string getNote() const {
        return note; }
    string getPayment() const {
        return payment; }

    // setter方法（用于修改记录）
    void setDate(Date d) { date = d; }
    void setAmount(double a) { amount = a; }
    void setCategory(const string& c) { category = c; }
    void setNote(const string& n) { note = n; }
    void setPayment(const string& p) { payment = p; }
};

// 收入记录类
class IncomeRecord : public FinanceRecord {

public:
    // 收入类别：工资、奖金、投资收益、兼职、礼金、其他
    static vector<string> getDefaultCategories() {
        return { "工资", "奖金", "投资权益", "兼职", "礼金", "其他" };
    }

    IncomeRecord() : FinanceRecord() {}
    IncomeRecord(const string& i, const Date& d, double a, const string& c, const string& n, const string& p)
        : FinanceRecord(i, d, a, c, n, p) {
    }

    string getType() const override { 
        return "收入"; }
    string toFileString() const override {
        return "收入|" + getID() + "|" + getDate().toString() + "|" +
            to_string(getAmount()) + "|" + getCategory() + "|" + getNote() + "|" + getPayment();
    }

    // 静态方法：从文件字符串解析收入记录
    static IncomeRecord fromFileString(const string& str) {
        stringstream ss;
		ss << str;
        string part;
        vector<string> parts;
        while (getline(ss, part, '|')) {
            parts.push_back(part);
        }
        Date date;
        Date::fromString(parts[2], date);
		IncomeRecord re= IncomeRecord(parts[1], date, stod(parts[3]), parts[4], parts[5], parts[6]);
        return re;
    }
};

// 支出记录类
class ExpenseRecord : public FinanceRecord {

public:
    // 支出类别：餐饮、交通、购物、住房、娱乐、医疗、教育、其他
    static vector<string> getDefaultCategories() {
		enum { DINING, TRANSPORT, SHOPPING, HOUSING, ENTERTAINMENT, MEDICAL, EDUCATION, OTHER };
        return { "餐饮", "交通", "购物", "住房", "娱乐", "医疗", "教育", "其他" };
    }

    ExpenseRecord() : FinanceRecord() {}
    ExpenseRecord(const string& i, const Date& d, double a, const string& c, const string& n, const string& p)
        : FinanceRecord(i, d, a, c, n, p) {
    }

    string getType() const override { return "支出"; }
    string toFileString() const override {
        return "支出|" + getID() + "|" + getDate().toString() + "|" + to_string(getAmount()) 
            + "|" + getCategory() + "|" + getNote() + "|" + getPayment();
    }

    // 静态方法：从文件字符串解析支出记录
    static ExpenseRecord fromFileString(const string& str) {
        stringstream ss;
		ss << str;
        string part;
        vector<string> parts;
        while (getline(ss, part, '|')) {
            parts.push_back(part);
        }
        Date date;
        Date::fromString(parts[2], date);
		ExpenseRecord re = ExpenseRecord(parts[1], date, stod(parts[3]), parts[4], parts[5], parts[6]);
        return re;
    }
};

// 财务管理类 核心
class FinanceManager {
private:
    vector<IncomeRecord> incomes;    // 收入记录集合
    vector<ExpenseRecord> expenses;  // 支出记录集合
    const string dataFile = "finance_data.txt";  // 数据存储文件

    // 生成唯一记录ID（年月日+序号）
    string generateID(const string& type) {
        Date now;
        string dateStr = now.toString();
        int count = 1;
        if (type == "收入") {
            for (size_t i = 0; i < incomes.size(); i++) {
                if (incomes[i].getDate().toString() == dateStr) 
                    count++;
            }
        }
        else {
            for (size_t i = 0; i < expenses.size();i++) {
                if (expenses[i].getDate().toString() == dateStr)
                    count++;
            }
        }
        return type + "-" + dateStr + "-" + to_string(count);
    }

    // 检查ID是否存在
    bool idExists(const string& id) {
        for (size_t i = 0; i < incomes.size(); i++) {
            if (incomes[i].getID() == id) 
                return true;
        }
        for (size_t i = 0; i < expenses.size(); i++) {
            if (expenses[i].getID() == id)
                return true;
        }
        return false;
    }

public:
    // 构造函数：加载数据
    FinanceManager() {
        loadData();
    }

    // 析构函数：保存数据
    ~FinanceManager() {
        saveData();
    }

    // 加载数据（从文件读取）
    void loadData() {
        ifstream file(dataFile);
		//检查文件是否存在
        if (!file.is_open()) {
            // 如果数据文件不存在，尝试创建一个空文件，保证后续备份/恢复能正常工作
            ofstream create(dataFile);
            if (create.is_open()) {
                create.close();
                cout << "首次使用，创建新数据文件！" << endl;
            }
            else {
                cout << "警告：无法创建数据文件！某些功能可能不可用。" << endl;
            }
            return;
        }

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            string type = line.substr(0, line.find('|'));
            if (type == "收入") {
                incomes.push_back(IncomeRecord::fromFileString(line));
            }
            else if (type == "支出") {
                expenses.push_back(ExpenseRecord::fromFileString(line));
            }
        }
        file.close();
        cout << "数据加载成功！共加载收入记录" << incomes.size() << "条，支出记录" << expenses.size() << "条。" << endl;
    }

    // 保存数据（写入文件）
    void saveData() {
        ofstream file(dataFile);
        if (!file.is_open()) {
            cout << "数据文件打开失败，数据未保存！" << endl;
            return;
        }

        // 写入收入记录
        for (size_t i = 0; i < incomes.size(); i++) {
            file << incomes[i].toFileString() << endl;
        }

        // 写入支出记录
        for (size_t i = 0; i < expenses.size(); i++) {
            file << expenses[i].toFileString() << endl;
        }
        file.close();
        cout << "数据保存成功！" << endl;
    }

    // 导出所有收入到指定文件（用于打印/导出功能）
    bool exportIncomes(const string& filename) {
        ofstream out(filename);
        if (!out.is_open()) {
            cout << "导出失败：无法打开文件 " << filename << endl;
            return false;
        }
        for (size_t i = 0; i < incomes.size(); i++) {
            out << incomes[i].toFileString() << endl;
        }
        out.close();
        cout << "收入记录已导出到：" << filename << endl;
        return true;
    }

    // 导出所有支出到指定文件（用于打印/导出功能）
    bool exportExpenses(const string& filename) {
        ofstream out(filename);
        if (!out.is_open()) {
            cout << "导出失败：无法打开文件 " << filename << endl;
            return false;
        }
        for (size_t i = 0; i < expenses.size(); i++) {
            out << expenses[i].toFileString() << endl;
        }
        out.close();
        cout << "支出记录已导出到：" << filename << endl;
        return true;
    }

    // 将所有记录导出为 CSV 文件（包含收入和支出）
    bool exportAllToCSV(const string& filename) {
        ofstream out(filename);
        if (!out.is_open()) {
            cout << "导出CSV失败：无法打开文件 " << filename << endl;
            return false;
        }

        // CSV 表头
        out << "类型,记录ID,日期,金额,类别,备注,支付方式" << endl;
		//lambda函数简化写入
        auto writeRow = [&out](const string& type, const FinanceRecord& r) {
			//lambda函数
            auto esc = [](const string& s) {
                // 转义双引号
                string t;
                for (char c : s) {
                    if (c == '"') t += '"', t += '"';
                    else t += c;
                }
                return string("\"") + t + string("\"");
            };
            out << esc(type) << "," << esc(r.getID()) << "," << esc(r.getDate().toString()) << "," << esc(to_string(r.getAmount()))
                << "," << esc(r.getCategory()) << "," << esc(r.getNote()) << "," << esc(r.getPayment()) << endl;
        };

		//写入所有记录
        for (const auto& rec : incomes) writeRow("收入", rec);
        for (const auto& rec : expenses) writeRow("支出", rec);
        out.close();
        cout << "已将所有记录导出为CSV：" << filename << endl;
        return true;
    }

    // 打印指定年份的每月收支柱状图（ASCII）到控制台
    void printMonthlyBarChart(int year) {
        vector<double> monthIncome(12, 0.0), monthExpense(12, 0.0);
        for (const auto& rec : incomes) {
            if (rec.getDate().getYear() == year) {
                monthIncome[rec.getDate().getMonth() - 1] += rec.getAmount();
            }
        }
        for (const auto& rec : expenses) {
            if (rec.getDate().getYear() == year) {
                monthExpense[rec.getDate().getMonth() - 1] += rec.getAmount();
            }
        }

        vector<double> net(12);
        double maxVal = 0.0;
        for (int i = 0; i < 12; i++) {
            net[i] = monthIncome[i] - monthExpense[i];
            maxVal = max(maxVal, fabs(net[i]));
        }
        if (maxVal < 1e-6) maxVal = 1.0; // 避免除零

        const int maxWidth = 50;
        cout << "\n" << year << " 年每月收支差额柱状图（正为净收入，负为净支出）：" << endl;
        for (int i = 0; i < 12; ++i) {
            int len = static_cast<int>(fabs(net[i]) / maxVal * maxWidth + 0.5);
            cout << setw(2) << (i + 1) << "月 ";
            if (net[i] >= 0) {
                cout << string(len, '+');
            }
            else {
                cout << string(len, '-');
            }
            cout << " " << fixed << setprecision(2) << net[i] << "元" << endl;
        }
        cout << endl;
    }

    // 可视化收入和支出趋势（ASCII折线图）
    void visualizeTrends(int year) {
        if (year < 1900 || year > 2100) {
            cout << "错误：年份无效！" << endl;
            return;
        }

        vector<double> monthIncome(12, 0.0), monthExpense(12, 0.0);
        for (const auto& rec : incomes) {
            if (rec.getDate().getYear() == year) {
                monthIncome[rec.getDate().getMonth() - 1] += rec.getAmount();
            }
        }
        for (const auto& rec : expenses) {
            if (rec.getDate().getYear() == year) {
                monthExpense[rec.getDate().getMonth() - 1] += rec.getAmount();
            }
        }

        cout << "\n" << year << " 年收入和支出趋势图：" << endl;
        const int maxWidth = 50;
        double maxVal = 0.0;
        for (int i = 0; i < 12; ++i) {
            maxVal = max(maxVal, max(monthIncome[i], monthExpense[i]));
        }
        if (maxVal < 1e-6) maxVal = 1.0; // 避免除零

        cout << "月份 收入趋势 支出趋势" << endl;
        for (int i = 0; i < 12; ++i) {
            int incomeLen = static_cast<int>(monthIncome[i] / maxVal * maxWidth + 0.5);
            int expenseLen = static_cast<int>(monthExpense[i] / maxVal * maxWidth + 0.5);
            cout << setw(2) << (i + 1) << "月 ";
            cout << string(incomeLen, '+') << " ";
            cout << string(expenseLen, '-') << endl;
        }
    }

    // 计算平均每月收入和支出
    void calculateAverageMonthlyIncomeExpense(int year) {
        if (year < 1900 || year > 2100) {
            cout << "错误：年份无效！" << endl;
            return;
        }

        vector<double> monthIncome(12, 0.0), monthExpense(12, 0.0);
        vector<int> monthDays(12, 0);

        for (size_t i = 0; i < incomes.size(); i++) {
            if (incomes[i].getDate().getYear() == year) {
                int month = incomes[i].getDate().getMonth() - 1;
                monthIncome[month] += incomes[i].getAmount();
                monthDays[month]++;
            }
        }
        for (size_t i = 0; i < expenses.size(); i++) {
            if (expenses[i].getDate().getYear() == year) {
                int month = expenses[i].getDate().getMonth() - 1;
                monthExpense[month] += expenses[i].getAmount();
                monthDays[month]++;
            }
        }

        double totalIncome = 0, totalExpense = 0;
        int activeMonths = 0;
        for (int i = 0; i < 12; i++) {
			//该月有记录才计入平均值计算
            if (monthDays[i] > 0) {
                totalIncome += monthIncome[i];
                totalExpense += monthExpense[i];
                activeMonths++;
            }
        }

        if (activeMonths == 0) {
            cout << "错误：指定年份没有任何记录！" << endl;
            return;
        }

        cout << year << " 年平均每月收入和支出：" << endl;
        cout << "平均每月收入：" << fixed << setprecision(2) << (totalIncome / activeMonths) << "元" << endl;
        cout << "平均每月支出：" << fixed << setprecision(2) << (totalExpense / activeMonths) << "元" << endl;
    }

    // 添加收入记录
    bool addIncome(double amount, const string& category, const string& note, const string& payment, const Date& date = Date()) {
        // 输入验证
        if (amount <= 0) {
            cout << "错误：收入金额必须大于0！" << endl;
            return false;
        }

		//检验收入类别是否有效
        vector<string> validCats = IncomeRecord::getDefaultCategories();
        bool catValid = false;
        for (const auto& c : validCats) {
            if (c == category) {
                catValid = true;
                break;
            }
        }
        if (!catValid) {
            cout << "错误：收入类别无效！支持的类别：";
            for (int i = 0; i < validCats.size(); i++) {
				//类别中间用顿号分隔
                cout << validCats[i] << (i == validCats.size() - 1 ? "" : "、");
            }
            cout << endl;
            return false;
        }
        if (!date.isValid()) {
            cout << "错误：日期格式无效！" << endl;
            return false;
        }

        // 生成ID并添加记录
        string id = generateID("收入");
        incomes.push_back(IncomeRecord(id, date, amount, category, note, payment));
        cout << "收入记录添加成功！记录ID：" << id << endl;
        return true;
    }

    // 添加支出记录
    bool addExpense(double amount, const string& category, const string& note, const string& payment, const Date& date = Date()) {
        // 输入验证
        if (amount <= 0) {
            cout << "错误：支出金额必须大于0！" << endl;
            return false;
        }
        //检查支出类别是否有效
        vector<string> validCats = ExpenseRecord::getDefaultCategories();
        bool catValid = false;
        for (const auto& c : validCats) {
            if (c == category) {
                catValid = true;
                break;
            }
        }
        if (!catValid) {
            cout << "错误：支出类别无效！支持的类别：";
            for (int i = 0; i < validCats.size(); i++) {
                cout << validCats[i] << (i == validCats.size() - 1 ? "" : "、");
            }
            cout << endl;
            return false;
        }
        if (!date.isValid()) {
            cout << "错误：日期格式无效！" << endl;
            return false;
        }

        // 生成ID并添加记录
        string id = generateID("支出");
        expenses.push_back(ExpenseRecord(id, date, amount, category, note, payment));
        cout << "支出记录添加成功！记录ID：" << id << endl;
        return true;
    }

    // 查询收入记录（多条件）
    vector<IncomeRecord> queryIncome(const string& id = "", const Date& startDate = Date(2000, 1, 1), 
        const Date& endDate = Date(2100, 12, 31), const string& category = "") {
        vector<IncomeRecord> result;
        for (size_t i = 0; i < incomes.size(); i++) {
            bool match = true;
            if (!id.empty() && incomes[i].getID() != id) 
                match = false;
            if (!(incomes[i].getDate() >= startDate && incomes[i].getDate() <= endDate)) 
                match = false;
            if (!category.empty() && incomes[i].getCategory() != category) 
                match = false;
            if (match) 
                result.push_back(incomes[i]);
        }
        return result;
    }

    // 查询支出记录（多条件）
    vector<ExpenseRecord> queryExpense(const string& id = "", const Date& startDate = Date(2000, 1, 1),
        const Date& endDate = Date(2100, 12, 31), const string& category = "") {
        vector<ExpenseRecord> result;
        for (size_t i = 0; i < expenses.size(); i++) {
            bool match = true;
            if (!id.empty() && expenses[i].getID() != id) 
                match = false;
            if (!(expenses[i].getDate() >= startDate && expenses[i].getDate() <= endDate)) 
                match = false;
            if (!category.empty() && expenses[i].getCategory() != category) 
                match = false;
            if (match) 
                result.push_back(expenses[i]);
        }
        return result;
    }

    // 修改收入记录
    bool modifyIncome(const string& id, double amount, const string& category, const string& note, const string& payment) {
        for (size_t i = 0; i < incomes.size(); i++) {
            if (incomes[i].getID() == id) {
                if (amount > 0) incomes[i].setAmount(amount);
                vector<string> validCats = IncomeRecord::getDefaultCategories();
                for (const auto& c : validCats) {
                    if (c == category) {
                        incomes[i].setCategory(category);
                        break;
                    }
                }
                incomes[i].setNote(note);
                incomes[i].setPayment(payment);
                cout << "收入记录修改成功！" << endl;
                return true;
            }
        }
        cout << "错误：未找到ID为" << id << "的收入记录！" << endl;
        return false;
    }

    // 修改支出记录
    bool modifyExpense(const string& id, double amount, const string& category, const string& note, const string& payment) {
        for (size_t i = 0; i < expenses.size(); i++) {
            if (expenses[i].getID() == id) {
                if (amount > 0) expenses[i].setAmount(amount);
                vector<string> validCats = ExpenseRecord::getDefaultCategories();
                for (size_t j = 0; j < validCats.size(); j++) {
                    if (validCats[j] == category) {
                        expenses[i].setCategory(category);
                        break;
                    }
                }
                expenses[i].setNote(note);
                expenses[i].setPayment(payment);
                cout << "支出记录修改成功！" << endl;
                return true;
            }
        }
        cout << "错误：未找到ID为" << id << "的支出记录！" << endl;
        return false;
    }

    // 删除收入记录
    bool deleteIncome(const string& id) {
        int i = 0;
		double amount = 0.0;
        for (auto it = incomes.begin(); it != incomes.end(); it++) {
            if (it->getID() == id) {
                incomes.erase(it);
                cout << "收入记录删除成功！" << endl;
                return true;
            }
        }
        cout << "错误：未找到ID为" << id << "的收入记录！" << endl;
        return false;
    }

    // 删除支出记录
    bool deleteExpense(const string& id) {
        for (auto it = expenses.begin(); it != expenses.end(); it++) {
            if (it->getID() == id) {
                expenses.erase(it);
                cout << "支出记录删除成功！" << endl;
                return true;
            }
        }
        cout << "错误：未找到ID为" << id << "的支出记录！" << endl;
        return false;
    }

    // 按日统计
    void statByDay(int year, int month, int day) {
        Date target(year, month, day);
        if (!target.isValid()) {
            cout << "错误：日期无效！" << endl;
            return;
        }

        double totalIncome = 0,totalExpense = 0;
        for (size_t i = 0; i < incomes.size(); i++) {
            Date d = incomes[i].getDate();
            if (d.getYear() == year && d.getMonth() == month && d.getDay() == day) {
                totalIncome += incomes[i].getAmount();
            }
        }
        for (size_t i = 0; i < expenses.size(); i++) {
            Date d = expenses[i].getDate();
            if (d.getYear() == year && d.getMonth() == month && d.getDay() == day) {
                totalExpense += expenses[i].getAmount();
            }
        }
        cout << year << "年" << month << "月" << day << "日统计结果：" << endl;
        cout << "总收入：" << fixed << setprecision(2) << totalIncome << "元" << endl;
        cout << "总支出：" << fixed << setprecision(2) << totalExpense << "元" << endl;
        cout << "收支差额：" << fixed << setprecision(2) << (totalIncome - totalExpense) << "元" << endl;
    }

    // 按月统计
    void statByMonth(int year, int month) {
        Date target(year, month, 1);
        if (!target.isValid()) {
            cout << "错误：年月无效！" << endl;
            return;
        }

        double totalIncome = 0,totalExpense = 0;

        // 收入类别统计
        vector<string> incomeCats = IncomeRecord::getDefaultCategories();
        vector<double> incomeCatAmounts(incomeCats.size(), 0);

        // 支出类别统计
        vector<string> expenseCats = ExpenseRecord::getDefaultCategories();
        vector<double> expenseCatAmounts(expenseCats.size(), 0);

        // 计算收入
        for (size_t i = 0; i < incomes.size(); i++) { // i：收支记录索引
            Date d = incomes[i].getDate();
            if (d.getYear() == year && d.getMonth() == month) {
                totalIncome += incomes[i].getAmount();
                // j：类别列表索引
                for (size_t j = 0; j < incomeCats.size(); j++) {
                    // 当前记录的类别 匹配 当前遍历的类别
                    if (incomes[i].getCategory() == incomeCats[j]) {
                        incomeCatAmounts[j] += incomes[i].getAmount();
                        break;
                    }
                }
            }
        }

        // 计算支出
        for (size_t i = 0; i < expenses.size(); i++) {
            Date d = expenses[i].getDate();
            if (d.getYear() == year && d.getMonth() == month) {
                totalExpense += expenses[i].getAmount();
				//按类别统计
                for (size_t j = 0; j < expenseCats.size(); j++) {
                    if (expenses[i].getCategory() == expenseCats[j]) {
                        expenseCatAmounts[j] += expenses[i].getAmount();
                        break;
                    }
                }
            }
        }

        // 输出统计结果
        cout << year << "年" << month << "月统计结果：" << endl;
        cout << "=" << string(30, '=') << endl;
		cout << "总收入：" << fixed << setprecision(2) << totalIncome << "元" << endl;//固定两位小数
        cout << "收入类别分布：" << endl;
        if (totalIncome == 0) {
            std::cout << "  本月无收入记录" << std::endl;
        }
        else {
            for (size_t i = 0; i < incomeCats.size(); i++) {
                if (incomeCatAmounts[i] > 0) {
                    double ratio = (incomeCatAmounts[i] / totalIncome) * 100;
                    cout << "  " << incomeCats[i] << "：" << fixed << setprecision(2)
                        << incomeCatAmounts[i] << "元（" << fixed << setprecision(1) << ratio << "%）" << endl;
                }
            }
        }
        cout << endl;
		cout << "总支出：" << fixed << setprecision(2) << totalExpense << "元" << endl;//固定两位小数
        cout << "支出类别分布：" << endl;
        if (totalExpense == 0) {
            std::cout << "  本月无收入记录" << std::endl;
        }
        else {
            for (size_t i = 0; i < expenseCats.size(); i++) {
                if (expenseCatAmounts[i] > 0) {
                    double ratio = (expenseCatAmounts[i] / totalExpense) * 100;
                    cout << "  " << expenseCats[i] << "：" << fixed << setprecision(2)
                        << expenseCatAmounts[i] << "元（" << fixed << setprecision(1) << ratio << "%）" << endl;
                }
            }
        }
        cout << endl;
        cout << "月收支差额：" << fixed << setprecision(2) << (totalIncome - totalExpense) << "元" << endl;
        cout << "=" << string(30, '=') << endl;
    }

    // 按年统计
    void statByYear(int year) {
        Date target(year, 1, 1);
        if (!target.isValid()) {
            cout << "错误：年份无效！" << endl;
            return;
        }
        double totalIncome = 0,totalExpense = 0;
        vector<double> monthIncome(12, 0), monthExpense(12, 0);

        // 计算每月收支
        for (size_t i = 0; i < incomes.size(); i++) {
            if (incomes[i].getDate().getYear() == year) {
                int month = incomes[i].getDate().getMonth() - 1;
                monthIncome[month] += incomes[i].getAmount();
                totalIncome += incomes[i].getAmount();
            }
        }
        for (size_t i = 0; i < expenses.size(); i++) {
            if (expenses[i].getDate().getYear() == year) {
                int month = expenses[i].getDate().getMonth() - 1;
                monthExpense[month] += expenses[i].getAmount();
                totalExpense += expenses[i].getAmount();
            }
        }

		// 输出结果,固定两位小数
        cout << year << "年统计结果：" << endl;
        cout << "=" << string(30, '=') << endl;
        cout << "全年总收入：" << fixed << setprecision(2) << totalIncome << "元" << endl;
        cout << "全年总支出：" << fixed << setprecision(2) << totalExpense << "元" << endl;
        cout << "全年收支差额：" << fixed << setprecision(2) << (totalIncome - totalExpense) << "元" << endl;
        cout << endl;
        cout << "各月收支明细：" << endl;
        cout << setw(8) << "月份" << setw(12) << "收入（元）" << setw(12) << "支出（元）" << setw(12) << "差额（元）" << endl;
        for (int i = 0; i < 12; i++) {
            double diff = monthIncome[i] - monthExpense[i];
            cout << setw(8) << (i + 1) << setw(12) << fixed << setprecision(2) << monthIncome[i]
                << setw(12) << fixed << setprecision(2) << monthExpense[i] << setw(12) << fixed << setprecision(2) << diff << endl;
        }
        cout << "=" << string(30, '=') << endl;
    }

	// 生成指定日期范围的摘要报告，包含总收入、总支出和收支差额
    void generateSummaryReport(const Date& startDate, const Date& endDate) {
        if (!startDate.isValid() || !endDate.isValid() || startDate > endDate) {
            cout << "错误：日期范围无效！" << endl;
            return;
        }

        double totalIncome = 0,totalExpense = 0;
        for (size_t i = 0; i < incomes.size(); i++) {
            if (incomes[i].getDate() >= startDate && incomes[i].getDate() <= endDate) {
                totalIncome += incomes[i].getAmount();
            }
        }
        for (size_t i = 0; i < expenses.size(); i++) {
            if (expenses[i].getDate() >= startDate && expenses[i].getDate() <= endDate) {
                totalExpense += expenses[i].getAmount();
            }
        }

        cout << "摘要报告（" << startDate.toString() << " 至 " << endDate.toString() << "）：" << endl;
        cout << "总收入：" << fixed << setprecision(2) << totalIncome << "元" << endl;
        cout << "总支出：" << fixed << setprecision(2) << totalExpense << "元" << endl;
        cout << "收支差额：" << fixed << setprecision(2) << (totalIncome - totalExpense) << "元" << endl;
    }

    // 打印记录列表（收入）
    void printIncomeList(const vector<IncomeRecord>& list) {
        if (list.empty()) {
            cout << "未找到符合条件的收入记录！" << endl;
            return;
        }
        cout << "收入记录列表（共" << list.size() << "条）：" << endl;
        cout << "=" << string(80, '=') << endl;
        cout << setw(15) << "记录ID" << setw(15) << "日期" << setw(12) << "金额（元）"
            << setw(10) << "类别" << setw(20) << "备注" << setw(10) << "支付方式" << endl;
        cout << "=" << string(80, '=') << endl;
        for (const auto& rec : list) {
            cout << setw(15) << rec.getID() << setw(15) << rec.getDate().toString()
                << setw(12) << fixed << setprecision(2) << rec.getAmount()
                << setw(10) << rec.getCategory() << setw(20) << rec.getNote()
                << setw(10) << rec.getPayment() << endl;
        }
        cout << "=" << string(80, '=') << endl;
    }

    // 打印记录列表（支出）
    void printExpenseList(const vector<ExpenseRecord>& list) {
        if (list.empty()) {
            cout << "未找到符合条件的支出记录！" << endl;
            return;
        }
        cout << "支出记录列表（共" << list.size() << "条）：" << endl;
        cout << "=" << string(80, '=') << endl;
        cout << setw(15) << "记录ID" << setw(15) << "日期" << setw(12) << "金额（元）"
            << setw(10) << "类别" << setw(20) << "备注" << setw(10) << "支付方式" << endl;
        cout << "=" << string(80, '=') << endl;
        for (const auto& rec : list) {
            cout << setw(15) << rec.getID() << setw(15) << rec.getDate().toString()
                << setw(12) << fixed << setprecision(2) << rec.getAmount()
                << setw(10) << rec.getCategory() << setw(20) << rec.getNote()
                << setw(10) << rec.getPayment() << endl;
        }
        cout << "=" << string(80, '=') << endl;
    }

    // 数据备份
    bool backupData(const string backupFile = "finance_backup.txt") {
        // 直接将内存中的记录写入备份文件
        ofstream dst(backupFile);
        if (!dst.is_open()) {
            cout << "备份失败！无法打开备份文件。" << endl;
            return false;
        }
        for (size_t i = 0; i < incomes.size(); i++) {
            dst << incomes[i].toFileString() << endl;
        }
        for (size_t i = 0; i < expenses.size(); i++) {
            dst << expenses[i].toFileString() << endl;
        }
        dst.close();
        cout << "数据备份成功！备份文件：" << backupFile << endl;
        return true;
    }

    // 数据恢复
    bool restoreData(const string backupFile = "finance_backup.txt") {
        ifstream src(backupFile);
        ofstream dst(dataFile);
        if (!src.is_open() || !dst.is_open()) {
            cout << "恢复失败！文件打开错误。" << endl;
            return false;
        }
        // 清空原有数据
        incomes.clear();
        expenses.clear();
        // 读取备份文件并同时写回主数据文件
        string line;
        while (getline(src, line)) {
            dst << line << endl;
            if (line.empty()) continue;
            string type = line.substr(0, line.find('|'));
            if (type == "收入" ) {
                incomes.push_back(IncomeRecord::fromFileString(line));
            }
            else if (type == "支出" ) {
                expenses.push_back(ExpenseRecord::fromFileString(line));
            }
        }
        src.close();
        dst.close();
        cout << "数据恢复成功！共恢复收入记录" << incomes.size() << "条，支出记录" << expenses.size() << "条。" << endl;
        return true;
    }
};

// 界面交互类
class UI {
private:
    FinanceManager manager;

    // 显示主菜单
    void showMainMenu() {
        system("cls");  // Windows清屏
        cout << "==================== 家庭财务小管家 ====================" << endl;
        cout << "1. 收入管理" << endl;
        cout << "2. 支出管理" << endl;
        cout << "3. 统计分析" << endl;
        cout << "4. 数据备份与恢复" << endl;
        cout << "5. 退出系统" << endl;
        cout << "======================================================" << endl;
        cout << "请选择功能（1-5）：";
    }

    // 显示收入管理菜单
    void showIncomeMenu() {
        system("cls");
        cout << "==================== 收入管理 ====================" << endl;
        cout << "1. 添加收入记录" << endl;
        cout << "2. 查询收入记录" << endl;
        cout << "3. 修改收入记录" << endl;
        cout << "4. 删除收入记录" << endl;
        cout << "5. 打印/导出记录" << endl;
        cout << "6. 返回主菜单" << endl;
        cout << "==================================================" << endl;
        cout << "请选择功能（1-6）：";
    }

    // 显示支出管理菜单
    void showExpenseMenu() {
        system("cls");
        cout << "==================== 支出管理 ====================" << endl;
        cout << "1. 添加支出记录" << endl;
        cout << "2. 查询支出记录" << endl;
        cout << "3. 修改支出记录" << endl;
        cout << "4. 删除支出记录" << endl;
        cout << "5. 打印/导出记录" << endl;
        cout << "6. 返回主菜单" << endl;
        cout << "==================================================" << endl;
        cout << "请选择功能（1-6）：";
    }

    // 显示统计分析菜单
    void showStatMenu() {
        system("cls");
        cout << "==================== 统计分析 ====================" << endl;
        cout << "1. 按日统计" << endl;
        cout << "2. 按月统计" << endl;
        cout << "3. 按年统计" << endl;
        cout << "4. 导出全部为CSV / 显示年度柱状图" << endl;
        cout << "5. 生成摘要报告" << endl;
        cout << "6. 计算平均每月收入和支出" << endl;
        cout << "7. 可视化收入和支出趋势" << endl;
        cout << "8. 返回主菜单" << endl;
        cout << "==================================================" << endl;
        cout << "请选择功能（1-8）：";
    }

    // 显示备份恢复菜单
    void showBackupMenu() {
        system("cls");
        cout << "==================== 数据备份与恢复 ====================" << endl;
        cout << "1. 备份数据" << endl;
        cout << "2. 恢复数据" << endl;
        cout << "3. 返回主菜单" << endl;
        cout << "======================================================" << endl;
        cout << "请选择功能（1-3）：";
    }

    // 输入整数（带范围验证）
    int inputInt(const string& prompt, int minVal, int maxVal) {
        int val;
        while (true) {
            cout << prompt;
            cin >> val;
			// 输入验证
            if (cin.fail() || val < minVal || val > maxVal) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "输入无效！请输入" << minVal << "-" << maxVal << "之间的整数。" << endl;
            }
            else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return val;
            }
        }
    }

    // 输入浮点数（带范围验证）
    double inputDouble(const string& prompt, double minVal) {
        double val;
        while (true) {
            cout << prompt;
            cin >> val;
			// 输入验证
            if (cin.fail() || val <= minVal) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "输入无效！请输入大于" << minVal << "的数字。" << endl;
            }
            else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return val;
            }
        }
    }

    // 输入字符串
    string inputString(const string& prompt) {
        string val;
        cout << prompt;
        getline(cin, val);
        return val;
    }

    // 输入日期
    Date inputDate() {
        int y, m, d;
        while (true) {
            cout << "请输入日期（格式：年 月 日，用空格分隔）：";
            cin >> y >> m >> d;
			// 输入验证
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "输入格式无效！请重新输入。" << endl;
            }
            else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                Date date(y, m, d);
                if (date.isValid()) {
                    return date;
                }
                else {
                    cout << "日期无效！请重新输入。" << endl;
                }
            }
        }
    }

public:
    // 运行界面
    void run() {
        cout << "欢迎使用家庭财务小管家！" << endl;
        system("pause");
        while (true) {
            showMainMenu();
            int choice = inputInt("", 1, 5);
            switch (choice) {
            case 1: handleIncomeMenu(); break;
            case 2: handleExpenseMenu(); break;
            case 3: handleStatMenu(); break;
            case 4: handleBackupMenu(); break;
            case 5:
                cout << "感谢使用，再见！" << endl;
                return;
            }
            system("pause");
        }
    }

    // 处理收入管理菜单
    void handleIncomeMenu() {
        while (true) {
            showIncomeMenu();
            int choice = inputInt("", 1, 6);
            switch (choice) {
            case 1: {
                double amount = inputDouble("请输入收入金额（元）：", 0);
                cout << "支持的收入类别：";
                vector<string> cats = IncomeRecord::getDefaultCategories();
                for (int i = 0; i < cats.size(); i++) {
                    cout << i + 1 << "." << cats[i] << "  ";
                }
                cout << endl;
                int catIdx = inputInt("请选择类别（序号）：", 1, cats.size());
                string category = cats[catIdx - 1];
                string note = inputString("请输入备注：");
                cout << "支持的支付方式：1.现金  2.微信  3.支付宝  4.银行卡" << endl;
                int payIdx = inputInt("请选择支付方式（序号）：", 1, 4);
                string payment[] = { "现金", "微信", "支付宝", "银行卡" };
                Date date = inputDate();
                manager.addIncome(amount, category, note, payment[payIdx - 1], date);
                break;
            }
            case 2: {
                cout << "查询方式：1.按ID查询  2.按日期范围查询  3.按类别查询  4.查询全部" << endl;
                int queryType = inputInt("请选择查询方式（序号）：", 1, 4);
                vector<IncomeRecord> result;
                if (queryType == 1) {
                    string id = inputString("请输入记录ID：");
                    result = manager.queryIncome(id);
                }
                else if (queryType == 2) {
                    cout << "请输入开始日期：" << endl;
                    Date start = inputDate();
                    cout << "请输入结束日期：" << endl;
                    Date end = inputDate();
                    result = manager.queryIncome("", start, end);
                }
                else if (queryType == 3) {
                    cout << "支持的收入类别：";
                    vector<string> cats = IncomeRecord::getDefaultCategories();
                    for (int i = 0; i < cats.size(); i++) {
                        cout << i + 1 << "." << cats[i] << "  ";
                    }
                    cout << endl;
                    int catIdx = inputInt("请选择类别（序号）：", 1, cats.size());
                    string category = cats[catIdx - 1];
                    result = manager.queryIncome("", Date(2000, 1, 1), Date(2100, 12, 31), category);
                }
                else {
                    result = manager.queryIncome();
                }
                manager.printIncomeList(result);
                break;
            }
            case 3: {
                string id = inputString("请输入要修改的收入记录ID：");
                double amount = inputDouble("请输入新金额（0元表示不修改）：", -1);
                cout << "支持的收入类别：";
                vector<string> cats = IncomeRecord::getDefaultCategories();

                for (int i = 0; i < cats.size(); i++) {
                    cout << i + 1 << "." << cats[i] << "  ";
                }
                cout << endl;
                int catIdx = inputInt("请选择新类别（序号0表示不修改）：", 0, cats.size());
                string category = (catIdx == 0) ? "" : cats[catIdx - 1];
                string note = inputString("请输入新备注（回车表示不修改）：");
                cout << "支持的支付方式：1.现金  2.微信  3.支付宝  4.银行卡  0.不修改" << endl;
                int payIdx = inputInt("请选择新支付方式（序号）：", 0, 4);
                string payment[] = { "", "现金", "微信", "支付宝", "银行卡" };
                manager.modifyIncome(id, amount, category, note, payment[payIdx]);
                break;
            }
            case 4: {
                string id = inputString("请输入要删除的收入记录ID：");
                manager.deleteIncome(id);
                break;
            }
            case 5: {
                string filename = inputString("请输入导出文件名（默认 finance_income_export.txt）：");
                if (filename.empty()) filename = "finance_income_export.txt";
                manager.exportIncomes(filename);
                break;
            }
            case 6: return;
            }
            system("pause");
        }
    }

    // 处理支出管理菜单
    void handleExpenseMenu() {
        while (true) {
            showExpenseMenu();
            int choice = inputInt("", 1, 6);
            switch (choice) {
            case 1: {
                double amount = inputDouble("请输入支出金额（元）：", 0);
                cout << "支持的支出类别：";
                vector<string> cats = ExpenseRecord::getDefaultCategories();

                for (int i = 0; i < cats.size(); i++) {
                    cout << i + 1 << "." << cats[i] << "  ";
                }
                cout << endl;

                int catIdx = inputInt("请选择类别（序号）：", 1, cats.size());
                string category = cats[catIdx - 1];
                string note = inputString("请输入备注：");
                cout << "支持的支付方式：1.现金  2.微信  3.支付宝  4.银行卡" << endl;
                int payIdx = inputInt("请选择支付方式（序号）：", 1, 4);
                string payment[] = { "现金", "微信", "支付宝", "银行卡" };
                Date date = inputDate();
                manager.addExpense(amount, category, note, payment[payIdx - 1], date);
                break;
            }
            case 2: {
                cout << "查询方式：1.按ID查询  2.按日期范围查询  3.按类别查询  4.查询全部" << endl;
                int queryType = inputInt("请选择查询方式（序号）：", 1, 4);
                vector<ExpenseRecord> result;
                if (queryType == 1) {
                    string id = inputString("请输入记录ID：");
                    result = manager.queryExpense(id);
                }
                else if (queryType == 2) {
                    cout << "请输入开始日期：" << endl;
                    Date start = inputDate();
                    cout << "请输入结束日期：" << endl;
                    Date end = inputDate();
                    result = manager.queryExpense("", start, end);
                }
                else if (queryType == 3) {
                    cout << "支持的支出类别：";
                    vector<string> cats = ExpenseRecord::getDefaultCategories();

                    for (int i = 0; i < cats.size(); i++) {
                        cout << i + 1 << "." << cats[i] << "  ";
                    }
                    cout << endl;
                    int catIdx = inputInt("请选择类别（序号）：", 1, cats.size());
                    string category = cats[catIdx - 1];
                    result = manager.queryExpense("", Date(2000, 1, 1), Date(2100, 12, 31), category);
                }
                else {
                    result = manager.queryExpense();
                }
                manager.printExpenseList(result);
                break;
            }
            case 3: {
                string id = inputString("请输入要修改的支出记录ID：");
                double amount = inputDouble("请输入新金额（0元表示不修改）：", -1);
                cout << "支持的支出类别：";
                vector<string> cats = ExpenseRecord::getDefaultCategories();

                for (int i = 0; i < cats.size(); i++) {
                    cout << i + 1 << "." << cats[i] << "  ";
                }
                cout << endl;
                int catIdx = inputInt("请选择新类别（序号0表示不修改）：", 0, cats.size());
                string category = (catIdx == 0) ? "" : cats[catIdx - 1];
                string note = inputString("请输入新备注（回车表示不修改）：");
                cout << "支持的支付方式：1.现金  2.微信  3.支付宝  4.银行卡  0.不修改" << endl;
                int payIdx = inputInt("请选择新支付方式（序号）：", 0, 4);
                string payment[] = { "", "现金", "微信", "支付宝", "银行卡" };
                manager.modifyExpense(id, amount, category, note, payment[payIdx]);
                break;
            }
            case 4: {
                string id = inputString("请输入要删除的支出记录ID：");
                manager.deleteExpense(id);
                break;
            }
            case 5: {
                string filename = inputString("请输入导出文件名（默认 finance_expense_export.txt）：");
                if (filename.empty()) filename = "finance_expense_export.txt";
                manager.exportExpenses(filename);
                break;
            }
            case 6: return;
            }
            system("pause");
        }
    }

    // 处理统计分析菜单
    void handleStatMenu() {
        while (true) {
            showStatMenu();
            int choice = inputInt("", 1, 8);
            switch (choice) {
            case 1: {
                int y = inputInt("请输入年份：", 2000, 2100);
                int m = inputInt("请输入月份：", 1, 12);
                int d = inputInt("请输入日期：", 1, 31);
                manager.statByDay(y, m, d);
                break;
            }
            case 2: {
                int y = inputInt("请输入年份：", 2000, 2100);
                int m = inputInt("请输入月份：", 1, 12);
                manager.statByMonth(y, m);
                break;
            }
            case 3: {
                int y = inputInt("请输入年份：", 2000, 2100);
                manager.statByYear(y);
                break;
            }
            case 4: {
                string filename = inputString("请输入导出CSV文件名（默认 finance_all_export.csv）：");
                if (filename.empty()) filename = "finance_all_export.csv";
                manager.exportAllToCSV(filename);
                int y = inputInt("请输入要显示柱状图的年份（2000-2100，0表示跳过）：", 0, 2100);
                if (y != 0) manager.printMonthlyBarChart(y);
                break;
            }
            case 5: {
                cout << "请输入开始日期：" << endl;
                Date start = inputDate();
                cout << "请输入结束日期：" << endl;
                Date end = inputDate();
                manager.generateSummaryReport(start, end);
                break;
            }
            case 6: {
                int y = inputInt("请输入年份：", 2000, 2100);
                manager.calculateAverageMonthlyIncomeExpense(y);
                break;
            }
            case 7: {
                int y = inputInt("请输入年份：", 2000, 2100);
                manager.visualizeTrends(y);
                break;
            }
            case 8: return;
            }
            system("pause");
        }
    }

    // 处理备份恢复菜单
    void handleBackupMenu() {
        while (true) {
            showBackupMenu();
            int choice = inputInt("", 1, 3);
            switch (choice) {
            case 1: manager.backupData();
                break;
            case 2: {
                cout << "警告：恢复数据将覆盖当前所有数据，是否继续？（1.是  2.否）" << endl;
                int confirm = inputInt("", 1, 2);
                if (confirm == 1) {
                    manager.restoreData();
                }
                else {
                    cout << "已取消恢复操作。" << endl;
                }
                break;
            }
            case 3: return;
            }
            system("pause");
        }
    }
};

// 主函数
int main() {
    UI ui;
    ui.run();
    return 0;
}

