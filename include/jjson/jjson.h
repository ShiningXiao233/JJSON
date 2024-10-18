#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace jjson {

using ::std::string;
using ::std::unordered_map;
using ::std::vector;

enum struct json_val_type {
    Null = 0,
    Dict,
    Number,
    String,
    Array
};

class json_t {

using json_str = string;
using json_dict = unordered_map<string, json_t*>;
using json_array = vector<json_t*>;

private:
    union _json_t
    {
        json_t* obj;
        json_str* str;
        json_dict* dict;
        json_array* array;
        
        _json_t(const json_str& x);
        _json_t(const json_dict& x);
        _json_t(const json_array& x);

        _json_t(json_val_type type = json_val_type::Null, const _json_t* other_d = nullptr);

        _json_t(json_val_type type, const _json_t& x);
        /**
         * hard copy for dict
         */
        void _json_dict_copy();
        /**
         * hard copy for array
         */
        void _json_array_copy();
        void destroy(json_val_type type);
    };
    
    json_val_type data_type;

    _json_t data;


    void set_null();
public:

    /**
     * a null json obj
     */
    static json_t Null_json;

    /**
     * 创建某个空的json_t
     */
    json_t(json_val_type type = json_val_type::Null);
    json_t(const string& _str);
    json_t(const json_t& other);
    json_t(json_t&& other);
    
    /**
     * 删掉所有的值
     */
    ~json_t();
    /**
     * 作为数组时使用
     */
    json_t& operator [] (size_t id);
    /**
     * 作为字典时使用
     */
    json_t& operator [] (const string& key);
    /**
     * 作为字符串时使用
     */
    const string get_string() const;

    bool set_key_val(const string& key, json_t&& val);
    bool set_key_val(const string& key, const json_t& val);
    bool set_key_val(const string& key, json_t* val);
    bool del_key(const string& key);
    
    bool set_idx_val(size_t idx, json_t* val);
    bool set_idx_val(size_t idx, json_t&& val);
    bool set_idx_val(size_t idx, const json_t& val);
    bool array_appond_val(json_t&& val);
    bool array_appond_val(const json_t& val);
    bool array_appond_val(json_t* val);
    
    json_val_type get_val_type();
    size_t array_size();
    
    string to_string();
};

}