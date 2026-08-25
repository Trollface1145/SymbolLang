#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <windows.h>
using namespace std;

// ---------- 常量 ----------
const string MSG_START = "修女疯狂祈祷中...";
const string MSG_END = "修女祈祷完成！一路畅通";
const string MSG_CREATE = "修女打开了一扇门：";
const string MSG_PUSH = "修女记下一段经文：";
const string MSG_OP = "修女研读经文：";
const string MSG_OUT = "修女宣读告示：";
const string MSG_UNKNOWN = "修女看不懂这句祈祷词：";
const string MSG_NO_STACK = "修女不知道你在说哪扇门，请先 @!";
const string MSG_INPUT_ERR = "这不是数字，修女无法超度它";
const string MSG_STACK_CONFUSION = "修女迷路了：未声明压入指定栈的操作（栈混淆）";
const string MSG_RET_MISS = "修女忘了写启示：函数缺少返回值";
const string MSG_FUNC_DEF = "修女记录了一篇经文：";
const string MSG_FUNC_RET = "修女得到启示：";
const string MSG_STACK_MISS = "这扇门不存在，修女找不到入口";
const string MSG_PTR_CREATE = "修女创造了一个指针：";
const string MSG_PTR_SET = "修女让指针 ";
const string MSG_PTR_DEREF = "修女读取指针 ";

// ---------- 数据结构 ----------
map<string, vector<double>> stacks;
map<string, double> vars;
map<string, string> pointerToVar;
string curStack = "";
vector<string> stackNames;

struct Func {
    vector<string> params;
    vector<string> body;
};
map<string, Func> funcs;

// ---------- 工具函数 ----------
string trim(const string& s) {
    size_t a = s.find_first_not_of(" \t\n\r");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\n\r");
    return s.substr(a, b - a + 1);
}

bool startWith(const string& s, const string& p) {
    return s.size() >= p.size() && s.compare(0, p.size(), p) == 0;
}

bool endWith(const string& s, const string& p) {
    return s.size() >= p.size() && s.compare(s.size() - p.size(), p.size(), p) == 0;
}

bool isNum(const string& s) {
    if (s.empty()) return false;
    char* end;
    strtod(s.c_str(), &end);
    return end == s.c_str() + s.size();
}

double toNum(const string& s) { return strtod(s.c_str(), nullptr); }

bool stackExist(const string& n) { return stacks.find(n) != stacks.end(); }
void createStack(const string& n) { if (!stackExist(n)) { stacks[n] = {}; stackNames.push_back(n); } }
void pushStack(const string& n, double v) { if (stackExist(n)) stacks[n].push_back(v); }
double peekStack(const string& n) { if (stackExist(n) && !stacks[n].empty()) return stacks[n].back(); return 0; }
bool varExist(const string& n) { return vars.find(n) != vars.end(); }
double getVar(const string& n) { return varExist(n) ? vars[n] : 0; }
void setVar(const string& n, double v) { vars[n] = v; }

// ---------- 指针相关 ----------
bool ptrExist(const string& n) { return pointerToVar.find(n) != pointerToVar.end(); }
string getPtrTarget(const string& n) { return ptrExist(n) ? pointerToVar[n] : ""; }
bool setPtrTarget(const string& ptrName, const string& varName) {
    if (!ptrExist(ptrName)) return false;
    if (!varExist(varName)) return false;
    pointerToVar[ptrName] = varName;
    return true;
}
double derefPtr(const string& ptrName) {
    if (!ptrExist(ptrName)) return 0.0;
    string target = pointerToVar[ptrName];
    if (target.empty() || !varExist(target)) return 0.0;
    return getVar(target);
}

// ---------- 表达式 ----------
string repVars(const string& e) {
    string r = e;
    for (auto kv : vars) {
        size_t p = r.find(kv.first);
        while (p != string::npos) {
            r.replace(p, kv.first.size(), to_string(kv.second));
            p = r.find(kv.first, p + to_string(kv.second).size());
        }
    }
    return r;
}

double evalExpr(const string& e) {
    string trimmed = trim(e);
    if (trimmed == "*#") return 1.0;
    if (trimmed == "/#") return 0.0;

    // 指针解引用：@* ptr
    if (startWith(trimmed, "@*")) {
        string ptrName = trim(trimmed.substr(2));
        if (ptrExist(ptrName)) {
            string target = getPtrTarget(ptrName);
            if (!target.empty() && varExist(target)) {
                return getVar(target);
            }
        }
        cout << "修女无法解引用指针 " << ptrName << endl;
        return 0.0;
    }

    string s = repVars(trimmed);
    vector<string> ops = {"++","--","*^","-%","<<=",">>=","===","<<",">>","<<-","->>"};
    for (auto op : ops) {
        size_t p = s.find(op);
        if (p != string::npos) {
            double L = toNum(trim(s.substr(0, p)));
            double R = toNum(trim(s.substr(p + op.size())));
            if (op=="++") return L*R;
            if (op=="--") return L/R;
            if (op=="*^") return pow(L,R);
            if (op=="-%") return (int)L % (int)R;
            if (op=="<<") return L>R ? 1 : 0;
            if (op==">>") return L<R ? 1 : 0;
            if (op=="===") return L==R ? 1 : 0;
            if (op=="<<=") return L>=R ? 1 : 0;
            if (op==">>=") return L<=R ? 1 : 0;
            if (op=="<<-") return L * pow(2, R);
            if (op=="->>") return (int)L / (int)pow(2, R);
        }
    }
    size_t p = s.find('+');
    if (p != string::npos) return toNum(trim(s.substr(0,p))) + toNum(trim(s.substr(p+1)));
    p = s.find('-');
    if (p != string::npos && p>0) return toNum(trim(s.substr(0,p))) - toNum(trim(s.substr(p+1)));
    return toNum(s);
}

bool evalCond(const string& e) { return evalExpr(e) != 0; }

// ---------- 提取函数 ----------
string getTargetStack(const string& s) {
    size_t a = s.find("<@ '");
    if (a != string::npos) {
        size_t b = s.find("'", a+4);
        if (b != string::npos) return trim(s.substr(a+4, b-a-4));
    }
    return "";
}
string getFuncName(const string& s) {
    size_t a = s.find("'");
    if (a != string::npos) {
        size_t b = s.find("'", a+1);
        if (b != string::npos) return trim(s.substr(a+1, b-a-1));
    }
    return "";
}
string getFuncParams(const string& s) {
    size_t a = s.find("("), b = s.find(")");
    if (a != string::npos && b != string::npos && b>a)
        return trim(s.substr(a+1, b-a-1));
    return "";
}
string getArgs(const string& s) { return getFuncParams(s); }
string getCond(const string& s) {
    size_t a = s.find("["), b = s.find("]");
    if (a != string::npos && b != string::npos && b>a)
        return trim(s.substr(a+1, b-a-1));
    return "";
}
void parsePush(const string& s, double& v, string& n) {
    size_t p = s.find("<");
    if (p != string::npos) {
        v = toNum(trim(s.substr(0,p)));
        n = trim(s.substr(p+1));
    } else {
        v = toNum(trim(s));
        n = "";
    }
}

// ---------- 前置声明 ----------
void execLine(const string& line);
void execBlock(const vector<string>& lines, int start, int end);
int findBrace(const vector<string>& lines, int start);
int findFuncEnd(const vector<string>& lines, int start);

// ---------- 执行块 ----------
void execBlock(const vector<string>& lines, int start, int end) {
    for (int i = start; i <= end && i < (int)lines.size(); i++) {
        string l = trim(lines[i]);
        if (l.empty() || startWith(l, "[/*")) continue;
        if (l == "<stop>") break;
        execLine(l);
    }
}

// ---------- 查找匹配的 } ----------
int findBrace(const vector<string>& lines, int start) {
    int cnt = 1;
    for (int i = start+1; i < (int)lines.size(); i++) {
        string l = trim(lines[i]);
        if (l == "{") cnt++;
        if (l == "}") { cnt--; if (cnt==0) return i; }
    }
    return (int)lines.size()-1;
}

// ---------- 查找函数结束 ----------
int findFuncEnd(const vector<string>& lines, int start) {
    int cnt = 0; bool ok = false;
    for (int i = start; i < (int)lines.size(); i++) {
        string l = trim(lines[i]);
        if (l == "{") { cnt++; ok = true; }
        else if (l == "}") {
            cnt--;
            if (ok && cnt == 0) {
                if (i+1 < (int)lines.size() && startWith(trim(lines[i+1]), "}*@>"))
                    return i+1;
                return i;
            }
        }
    }
    return (int)lines.size()-1;
}

// ---------- 执行函数体 ----------
void execFuncBody(const Func& f, const vector<string>& args) {
    auto oldStacks = stacks;
    auto oldVars = vars;
    auto oldNames = stackNames;
    auto oldPtrs = pointerToVar;
    string oldCur = curStack;
    stacks.clear(); vars.clear(); stackNames.clear(); pointerToVar.clear(); curStack = "";

    for (size_t i = 0; i < f.params.size() && i < args.size(); i++)
        setVar(f.params[i], evalExpr(args[i]));

    bool hasRet = false; double retVal = 0;
    for (const string& line : f.body) {
        string l = trim(line);
        if (l.empty()) continue;
        if (startWith(l, "}*@>")) {
            string e = trim(l.substr(4));
            if (!e.empty()) { retVal = evalExpr(e); hasRet = true; }
            else cout << MSG_RET_MISS << endl;
            break;
        }
        execLine(l);
    }

    stacks = oldStacks; vars = oldVars; stackNames = oldNames; pointerToVar = oldPtrs; curStack = oldCur;
    if (hasRet && !curStack.empty() && stackExist(curStack)) {
        pushStack(curStack, retVal);
        cout << MSG_FUNC_RET << retVal << endl;
    }
}

// ---------- 执行函数调用 ----------
void execFuncCall(const string& line) {
    string name = getFuncName(line);
    string args = getArgs(line);
    vector<string> argList;
    if (!args.empty()) {
        stringstream ss(args); string a;
        while (getline(ss, a, ',')) argList.push_back(trim(a));
    }
    auto it = funcs.find(name);
    if (it == funcs.end()) { cout << "修女找不到这篇经文：" << name << endl; return; }
    execFuncBody(it->second, argList);
}

// ---------- 核心执行函数 ----------
void execLine(const string& line) {
    string l = trim(line);
    if (l.empty() || startWith(l, "[/*") || l=="{" || l=="}") return;

    // ----- !!@ 创建指针（必须放在 @! 之前）-----
    if (startWith(l, "!!@")) {
        string ptrName = trim(l.substr(3));
        if (ptrName.empty()) {
            cout << "修女不知道要创建什么指针" << endl;
            return;
        }
        if (ptrExist(ptrName)) {
            cout << "指针 " << ptrName << " 已存在" << endl;
            return;
        }
        pointerToVar[ptrName] = "";
        cout << MSG_PTR_CREATE << ptrName << endl;
        return;
    }

    // ----- @& 设置指针指向变量（必须放在 @! 之前）-----
    if (startWith(l, "@&")) {
        string rest = trim(l.substr(2));
        size_t ltPos = rest.find("<");
        if (ltPos == string::npos) {
            cout << "修女找不到 < 符号" << endl;
            return;
        }
        string ptrName = trim(rest.substr(0, ltPos));
        string varName = trim(rest.substr(ltPos + 1));
        if (ptrName.empty() || varName.empty()) {
            cout << "修女看不懂指针设置" << endl;
            return;
        }
        if (!ptrExist(ptrName)) {
            cout << "指针 " << ptrName << " 不存在" << endl;
            return;
        }
        if (!varExist(varName)) {
            cout << "变量 " << varName << " 不存在" << endl;
            return;
        }
        pointerToVar[ptrName] = varName;
        cout << MSG_PTR_SET << ptrName << " 指向变量 " << varName << endl;
        return;
    }

    // ----- @# 布尔变量 -----
    if (startWith(l, "@#")) {
        string rest = trim(l.substr(2));
        string varName;
        string initValue = "/#";
        size_t eqPos = rest.find('=');
        if (eqPos != string::npos) {
            varName = trim(rest.substr(0, eqPos));
            initValue = trim(rest.substr(eqPos + 1));
        } else {
            varName = trim(rest);
        }
        if (varName.empty()) {
            cout << "修女不知道要创建什么变量" << endl;
            return;
        }
        double val = 0.0;
        if (initValue == "*#") val = 1.0;
        else if (initValue == "/#") val = 0.0;
        else val = evalExpr(initValue);
        setVar(varName, val);
        cout << "修女创建了布尔变量：" << varName << " = " << (val ? "真" : "假") << endl;
        return;
    }

    // ----- @! 创建栈 -----
    if (startWith(l, "@!")) {
        string n = trim(l.substr(2));
        createStack(n); curStack = n;
        cout << MSG_CREATE << n << endl;
        return;
    }

    // @->
    if (startWith(l, "@->")) {
        string n = trim(l.substr(3));
        if (stackExist(n)) { curStack = n; cout << "切换到栈: " << n << endl; }
        else cout << "栈 " << n << " 不存在" << endl;
        return;
    }

    // @*~
    if (startWith(l, "@*~")) {
        string rest = trim(l.substr(3));
        string target = getTargetStack(rest);
        if (!target.empty()) {
            size_t p = rest.find("<@ '");
            if (p != string::npos) rest = trim(rest.substr(0, p));
        }
        if (curStack.empty() && target.empty()) {
            if (stackNames.size() > 1) { cout << MSG_STACK_CONFUSION << endl; return; }
            else if (stackNames.size() == 1) curStack = stackNames[0];
            else { cout << MSG_NO_STACK << endl; return; }
        }
        double v; string n; parsePush(rest, v, n);
        string s = target.empty() ? curStack : target;
        if (!stackExist(s)) { cout << "栈 " << s << " 不存在" << endl; return; }
        pushStack(s, v);
        if (!n.empty()) { setVar(n, v); cout << MSG_PUSH << v << " -> " << n << " (栈:" << s << ")" << endl; }
        else cout << MSG_PUSH << v << " (栈:" << s << ")" << endl;
        return;
    }

    // @!!~
    if (startWith(l, "@!!~")) {
        string e = trim(l.substr(4));
        string target = getTargetStack(e);
        if (!target.empty()) {
            size_t p = e.find("<@ '");
            if (p != string::npos) e = trim(e.substr(0, p));
        }
        if (curStack.empty() && target.empty()) {
            if (stackNames.size() > 1) { cout << MSG_STACK_CONFUSION << endl; return; }
            else if (stackNames.size() == 1) curStack = stackNames[0];
            else { cout << MSG_NO_STACK << endl; return; }
        }
        double r = evalExpr(e);
        string s = target.empty() ? curStack : target;
        if (!stackExist(s)) { cout << "栈 " << s << " 不存在" << endl; return; }
        pushStack(s, r);
        cout << MSG_OP << e << " = " << r << " (栈:" << s << ")" << endl;
        return;
    }

    // @&&
    if (startWith(l, "@&&")) {
        string n = trim(l.substr(3));
        if (!stackExist(n)) { cout << MSG_STACK_MISS << endl; return; }
        cout << MSG_OUT << n << "站台客流为" << peekStack(n) << "人，请有序出站" << endl;
        return;
    }

    // @<
        // @>
    if (startWith(l, "@>")) {
        string c = trim(l.substr(2));
        // 如果内容以双引号开头，直接输出字符串
        if (startWith(c, "\"") && endWith(c, "\"")) {
            cout << c.substr(1, c.size()-2) << endl;
            return;
        }
        // 否则作为表达式求值输出
        double val = evalExpr(c);
        cout << val << endl;
        return;
    }

    // @>
if (startWith(l, "@>")) {
    string c = trim(l.substr(2));
    // 如果内容以双引号开头，直接输出字符串
    if (startWith(c, "\"") && endWith(c, "\"")) {
        cout << c.substr(1, c.size()-2) << endl;
        return;
    }
    // 否则尝试作为表达式求值
    double val = evalExpr(c);
    cout << val << endl;
    return;
}

    cout << MSG_UNKNOWN << l << endl;
}

// ---------- 主函数 ----------
int main(int argc, char* argv[]) {
    SetConsoleOutputCP(936);
    string fname;
    if (argc > 1) fname = argv[1];
    else { cout << "请输入文件名: "; getline(cin, fname); }

    ifstream file(fname);
    if (!file.is_open()) { cerr << "无法打开文件: " << fname << endl; return 1; }

    vector<string> lines;
    string line;
    while (getline(file, line)) lines.push_back(line);
    file.close();

    cout << MSG_START << endl;

    stacks.clear(); vars.clear(); stackNames.clear(); funcs.clear(); pointerToVar.clear(); curStack = "";

    // 收集函数定义
    for (int i = 0; i < (int)lines.size(); i++) {
        string l = trim(lines[i]);
        if (startWith(l, "*@'") && l.find('(') != string::npos) {
            string name = getFuncName(l);
            string params = getFuncParams(l);
            int end = findFuncEnd(lines, i+1);
            if (end > i+1) {
                Func f;
                if (!params.empty()) {
                    stringstream ss(params); string p;
                    while (getline(ss, p, ',')) f.params.push_back(trim(p));
                }
                for (int j = i+1; j < end; j++) f.body.push_back(lines[j]);
                funcs[name] = f;
                cout << MSG_FUNC_DEF << name << "(" << params << ")" << endl;
                i = end;
            }
        }
    }

    // 执行代码
    for (int i = 0; i < (int)lines.size(); i++) {
        string l = trim(lines[i]);
        if (l.empty() || startWith(l, "[/*")) continue;

        if (startWith(l, "*@'") && l.find('(') != string::npos) {
            continue;
        }

        if (startWith(l, "*@'") && l.find('(') != string::npos) {
            execFuncCall(l);
            continue;
        }

        if (startWith(l, "/") && !startWith(l, "/*")) {
            string cond = getCond(l);
            if (!cond.empty()) {
                bool res = evalCond(cond);
                int s = i+1;
                while (s < (int)lines.size() && trim(lines[s]).empty()) s++;
                if (s < (int)lines.size() && trim(lines[s]) == "{") {
                    int e = findBrace(lines, s);
                    if (res) execBlock(lines, s+1, e-1);
                    else {
                        int elsePos = -1;
                        for (int j = e+1; j < (int)lines.size(); j++) {
                            if (trim(lines[j]) == "(**)") { elsePos = j; break; }
                        }
                        if (elsePos != -1) {
                            int es = elsePos+1;
                            while (es < (int)lines.size() && trim(lines[es]).empty()) es++;
                            if (es < (int)lines.size() && trim(lines[es]) == "{") {
                                int ee = findBrace(lines, es);
                                execBlock(lines, es+1, ee-1);
                                i = ee;
                            }
                        }
                        i = e;
                    }
                }
            }
            continue;
        }

        if (startWith(l, "-/*")) {
            string cond = getCond(l);
            if (!cond.empty()) {
                int s = i+1;
                while (s < (int)lines.size() && trim(lines[s]).empty()) s++;
                if (s < (int)lines.size() && trim(lines[s]) == "{") {
                    int e = findBrace(lines, s);
                    while (evalCond(cond)) execBlock(lines, s+1, e-1);
                    i = e;
                }
            }
            continue;
        }

        if (startWith(l, "(/)")) {
            string expr = getCond(l);
            if (!expr.empty()) {
                size_t p = expr.find("<");
                if (p != string::npos) {
                    string vn = trim(expr.substr(0, p));
                    int cnt = (int)toNum(trim(expr.substr(p+1)));
                    int s = i+1;
                    while (s < (int)lines.size() && trim(lines[s]).empty()) s++;
                    if (s < (int)lines.size() && trim(lines[s]) == "{") {
                        int e = findBrace(lines, s);
                        for (int k = 0; k < cnt; k++) {
                            setVar(vn, k);
                            execBlock(lines, s+1, e-1);
                        }
                        i = e;
                    }
                }
            }
            continue;
        }

        execLine(l);
    }

    cout << MSG_END << endl;
    return 0;
}