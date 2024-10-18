#include "jjson.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <iostream>

namespace jjson {

using std::unique_ptr;

json_t json_t::Null_json{json_val_type::Null};

void json_t::set_null()
{
    data_type = json_val_type::Null;
    data.obj = nullptr;
}

json_t::json_t(json_val_type type) : data_type(type),
                                     data(type)
{
    // std::cout << "id creat" << std::endl;
}

json_t::json_t(const string &_str) : data_type(json_val_type::String), data(_str)
{
    // std::cout << "str creat" << std::endl;

}

json_t::json_t(const json_t &other) : data_type(other.data_type),
                                      data(other.data_type, other.data)
{
    // std::cout << "copy creat" << std::endl;

}

json_t::json_t(json_t &&other) : data_type(other.data_type), data(other.data)
{
    // std::cout << "move creat" << std::endl;
    other.set_null();
}

json_t::~json_t()
{
    data.destroy(data_type);
}

json_t &jjson::json_t::operator[](size_t id)
{
    if (data_type != json_val_type::Array || id >= data.array->size()) {
        return Null_json;
    }
    return *(data.array->at(id));
}

json_t &json_t::operator[](const string &key)
{
    if (data_type != json_val_type::Dict || 
        data.dict->find(key) == data.dict->end()) {
        return Null_json;
    }
    return *(data.dict->at(key));
}

const string json_t::get_string() const
{
    if (data_type != json_val_type::String) {
        return "";
    }
    return static_cast<const string>(*(data.str));
}

bool json_t::set_key_val(const string &key, json_t &&val)
{
    using ::std::make_pair;
    if (data_type != json_val_type::Dict) {
        return false;
    }
    if (data.dict->find(key) == data.dict->end()) {
        data.dict->insert(make_pair(key, new json_t(::std::move(val))));
    } else {
        if (data.dict->at(key) == &val) return true;
        else {
            delete data.dict->at(key);
            data.dict->at(key) = new json_t(::std::move(val));
        }
    }
    return true;
}

bool json_t::set_key_val(const string &key, const json_t &val)
{
    using ::std::make_pair;

    if (data_type != json_val_type::Dict) {
        return false;
    }
    if (data.dict->find(key) == data.dict->end()) {
        data.dict->insert(make_pair(key, new json_t(val)));
    } else {
        if (data.dict->at(key) == &val) return true;
        else {
            delete data.dict->at(key);
            data.dict->at(key) = new json_t(val);
        }
    }
    return true;
}

bool json_t::set_key_val(const string &key, json_t *val)
{
    using ::std::make_pair;

    if (data_type != json_val_type::Dict) {
        return false;
    }
    if (data.dict->find(key) == data.dict->end()) {
        data.dict->insert(make_pair(key, val));
    } else {
        if (data.dict->at(key) == val) return true;
        else {
            delete data.dict->at(key);
            data.dict->at(key) = val;
        }
    }
    return true;
}

bool json_t::del_key(const string &key)
{
    if (data_type != json_val_type::Dict) {
        return false;
    }
    if (data.dict->find(key) == data.dict->end()) return true;
    else {
        delete data.dict->at(key);
        data.dict->erase(key);
        return true;
    }
    return false;
}

bool json_t::set_idx_val(size_t idx, json_t *val)
{
    if (data_type != json_val_type::Array || data.array->size() <= idx) {
        return false;
    }

    if (data.array->at(idx) == val) return true;
    else {
        delete data.array->at(idx);
        data.array->at(idx) = val;
    }

    return true;
}

bool json_t::set_idx_val(size_t idx, json_t &&val)
{
    if (data_type != json_val_type::Array || data.array->size() <= idx) {
        return false;
    }

    if (data.array->at(idx) == &val) return true;
    else {
        delete data.array->at(idx);
        data.array->at(idx) = new json_t(::std::move(val));
    }

    return true;
}

bool json_t::set_idx_val(size_t idx, const json_t &val)
{
    if (data_type != json_val_type::Array || data.array->size() <= idx) {
        return false;
    }

    if (data.array->at(idx) == &val) return true;
    else {
        delete data.array->at(idx);
        data.array->at(idx) = new json_t(val);
    }

    return true;
}

bool json_t::array_appond_val(json_t &&val)
{
    return array_appond_val(new json_t(::std::move(val)));
}

bool json_t::array_appond_val(const json_t &val)
{
    return array_appond_val(new json_t(val));
}

bool json_t::array_appond_val(json_t *val)
{
    if (data_type != json_val_type::Array) {
        return false;
    }
    data.array->push_back(val);
    return true;
}

json_val_type json_t::get_val_type()
{
    return data_type;
}

size_t json_t::array_size()
{
    if (data_type == json_val_type::Array) 
        return data.array->size();
    return static_cast<size_t>(0);
}

string json_t::to_string()
{
    string res{};
    switch (data_type)
    {
    case json_val_type::Null:
        break;
    case json_val_type::Array:
        res += "[";
        for (auto it = data.array->begin(); it != data.array->end(); ++it) {
            if (it != data.array->begin()) res += ',';
            res += (*it)->to_string();
        }
        res += "]";
        break;
    case json_val_type::Dict:
        res += "{";
        for (auto it = data.dict->begin(); it != data.dict->end(); ++it) {
            if (it != data.dict->begin()) res += ',';
            res += "\"" + (*it).first + "\":" + (*it).second->to_string();
        }
        res += "}";
        break;
    case json_val_type::String:
        res = "\"" + *data.str + '\"';
        break;
    default:
        break;
    }
    return res;
}

json_t::_json_t::_json_t(const json_str &x) : str(new json_str(x)) {
    
}

json_t::_json_t::_json_t(const json_dict &x) : dict(new json_dict(x))
{
    _json_dict_copy();
}

json_t::_json_t::_json_t(const json_array &x) : array(new json_array(x))
{
    _json_array_copy();
}

json_t::_json_t::_json_t(json_val_type type, const _json_t* other_d)
{
    switch (type)   
    {
    case json_val_type::Null: 
        obj = nullptr;
        break;
    case json_val_type::String:
        if (other_d == nullptr) {
            str = new json_str();
        }
        else {
            str = new json_str(*other_d->str);
        }
        break;
    case json_val_type::Dict:
        if (other_d == nullptr)
            dict = new json_dict();
        else {
            dict = new json_dict(*other_d->dict);
            _json_dict_copy();
        }
        break;
    case json_val_type::Array:
        if (other_d == nullptr)
            array = new json_array();
        else {
            array = new json_array(*other_d->array);
            _json_array_copy();

        }
        break;
    default:
        break;
    }
}

json_t::_json_t::_json_t(json_val_type type, const _json_t &x)
{
    switch (type)   
    {
    case json_val_type::Null: 
        obj = nullptr;
        break;
    case json_val_type::String:
        str = new json_str(*x.str);
        break;
    case json_val_type::Dict:
        dict = new json_dict(*x.dict);
        _json_dict_copy();
        break;
    case json_val_type::Array:
        array = new json_array(*x.array);
        _json_array_copy();
        break;
    default:
        break;
    }
}

void json_t::_json_t::_json_dict_copy()
{
    for (auto it = dict->begin(); it != dict->end(); ++it) {
        it->second = new json_t(*(it->second));
    }
}

void json_t::_json_t::_json_array_copy()
{
    for (auto it = array->begin(); it != array->end(); ++it) {
        *it = new json_t(*(*it));
    }
}

void json_t::_json_t::destroy(json_val_type type)
{
    switch (type)   
    {
    case json_val_type::Null: 
        break;
    case json_val_type::String:
        delete str;
        break;
    case json_val_type::Dict:
    /**
     * 递归删掉所有的值
     */
        for (auto it = dict->begin(); it != dict->end(); ++it) {
            delete (*it).second;
        }
        delete dict;
        break;
    case json_val_type::Array:
    /**
     * 循环删掉所有的值
     */
        for (auto it = array->begin(); it != array->end(); ++it) {
            delete (*it);
        }
        delete array;
        break;
    default:
        break;
    }
}
}
