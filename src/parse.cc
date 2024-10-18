#include "parse.h"

#include <cstring>
#include <stack>
#include <iostream>
#include <fstream>
#include <sstream>

std::string get_string(const char* &p, const char *end) {
    std::string res{};
    if (*p == '\"') {
        ++p;
        while (*p != '\"' && p < end) {
            if ((*p) == '\\') {
                ++p;
                if (p >= end) {
                    break;
                }
            }
            res += (*p);
            ++p;
        }
        ++p;

    } else {
        while (p < end && (((*p) >= '0' && (*p) <= '9') || (*p) == '.')) {
            res += (*p);
            ++p;
        }
    }
    return res;
}

namespace jjson {

json_t parse(const char * cbegin, const char * cend)
{   
    using ::std::cout;
    using ::std::endl;
    // ::std::cout << " --- " << cbegin << ::std::endl;
    using ::std::stack;
    struct json_node {
        json_t * jobj;
        enum STAT : u_int8_t {
            wait_key = 0,
            wait_colon,
            wait_val,
            wait_over,
            over
        } status;
        string key;
        json_node(json_t * obj, STAT st = STAT::wait_key) : jobj(obj), status(st), key() {}
    };
    stack<json_node*> stk;
    cbegin += strspn(cbegin, " \n\t"); // 跳过空白字符
    // ::std::cout << *cbegin << ::std::endl;
    json_node* now;
    if (*cbegin == '{') // 顶层是 dict
        now = new json_node(new json_t(json_val_type::Dict));
    else if (*cbegin == '[') { // 顶层是 array
        now = new json_node(new json_t(json_val_type::Array), json_node::STAT::wait_val);
    } else {
        return json_t();
    }
    cbegin ++;
    json_node* tmp;

    auto comb_json = [](json_node* a, json_node* b) -> void { // 将 b 并为 a 的子节点
        switch (a->jobj->get_val_type())
        {
        case json_val_type::Array:
            a->jobj->array_appond_val(b->jobj);
            break;
        case json_val_type::Dict:
            a->jobj->set_key_val(a->key, b->jobj);
            break;
        default:
            break;
        }
    };
    bool f_error = false; // 语法格式错误标记

    while (cbegin != cend) {
        cbegin += strspn(cbegin, " \n\t");
        if (*cbegin == ']') { // 解决空[]的问题
            if (now->jobj->get_val_type() == json_val_type::Array) {
                if (now->status == json_node::STAT::wait_val) {
                    now->status = json_node::STAT::over;
                    ++cbegin; 
                    // cbegin += strspn(cbegin, " \n\t"); // 跳过空白字符
                    continue;

                }
            }
        }
        if (*cbegin == '}') { // 解决空{}的问题
            if (now->jobj->get_val_type() == json_val_type::Dict) {
                if (now->status == json_node::STAT::wait_key) {
                    now->status = json_node::STAT::over;
                    ++cbegin;
                    // cbegin += strspn(cbegin, " \n\t");
                    continue;
                }
            }
        }
        switch(now->status) {
        case json_node::STAT::wait_key: // 等待key
            now->key = get_string(cbegin, cend);
            if (now->key == "") {
                f_error = true;
                // cout << " --- " << endl;
            }
            now->status = json_node::STAT::wait_colon;
            break;
        case json_node::STAT::wait_colon: // 已经获取到key，等待冒号
            if (*cbegin == ':') {
                now->status = json_node::STAT::wait_val;
            } else {
                f_error = true;
                // cout << "--:" << endl;
            }
            ++cbegin;
            break;
        case json_node::STAT::wait_val: // 等待val
            switch (*cbegin)
            {
            case '[': // new array
                stk.push(now);
                now = new json_node(new json_t(json_val_type::Array), json_node::STAT::wait_val);
                ++cbegin;
                break;
            case '{': // new dict
                stk.push(now);
                now = new json_node(new json_t(json_val_type::Dict), json_node::STAT::wait_key);
                ++cbegin;
                break;
            default: // new (str)
                stk.push(now);
                auto tmp_str = get_string(cbegin, cend);
                if (tmp_str == "") {
                    f_error = true;
                    // cout << "--++--" << endl;
                    break;
                }
                now = new json_node(new json_t(tmp_str), json_node::STAT::over);
                break;
            }
            break;
        
        case json_node::STAT::wait_over: // 等待结束符 ']' '}'
            switch (*cbegin)
            {
            case ']':
                if (now->jobj->get_val_type() == json_val_type::Array) {
                    now->status = json_node::STAT::over;
                } else {
                    f_error = true;
                    // cout << " ++ " << endl;
                }
                break;
            case '}':
                if (now->jobj->get_val_type() == json_val_type::Dict) {
                    now->status = json_node::STAT::over;
                } else {
                    f_error = true;
                    // cout << " ++ " << endl;

                }
                break;
            default:
                f_error = true;
                // cout << "___" << endl;
                break;
            }
            ++cbegin;
            break;
        case json_node::STAT::over:
            if (stk.empty() && cbegin == cend) { // 特殊情况1：顶层为[]或者{}
                break;
            }
            else if (stk.empty()) {
                f_error = true;
                // cout << int(*cbegin) << endl;
                // cout << (cbegin == cend) << endl;
                // cout << "+++" << endl;
            } else {
                tmp = stk.top(); stk.pop();
                comb_json(tmp, now);
                now = tmp;
            }

            if (cbegin < cend && *cbegin == ',') {
                cbegin ++;
                if (now->jobj->get_val_type() == json_val_type::Array) {
                    now->status = json_node::STAT::wait_val;
                } else if (now->jobj->get_val_type() == json_val_type::Dict) {
                    now->status = json_node::STAT::wait_key;
                } else {
                    f_error = true;
                }
            } else {
                now->status = json_node::STAT::wait_over;
            }
            break;
        default:
            break;
        }
        
        if (f_error) break;
        cbegin += strspn(cbegin, " \n\t");

    }
    // cout << f_error << endl;

    if (!stk.empty()) f_error = true;
    // cout << f_error << endl;
    if (f_error) { // 存在错误
        delete now->jobj;
        while (!stk.empty()) {
            delete stk.top()->jobj;
            stk.pop();
        }
        return json_t();
    }
    
    return ::std::move(*(now->jobj));
}

json_t parse(const string &x)
{
    return parse(x.c_str(), x.c_str() + x.size());
}

json_t parse(ifstream &x)
{
    ::std::stringstream tmp;
    tmp << x.rdbuf();
    return parse(tmp.str());
}

json_t jjson_utils::operator""_json(const char * c, size_t n)
{
    return parse(c, c + n);
}

}


