//  Copyright John R. Bandela 2012
//
// Distributed under the Boost Software License, Version 1.0.
//    (See  http://www.boost.org/LICENSE_1_0.txt)

// Class value modeled on facebook folly::dynamic but uses boost and compiles under MSVC 11
#pragma once

#ifndef JRB_JSON_03_07_2012
#define JRB_JSON_03_07_2012

#include<string>
#include <boost/variant.hpp>
#include <vector>
#include <map>
#include <memory>
#include <boost/cstdint.hpp>
#include <utility>
#include <stdexcept>
#include <boost/lexical_cast.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include <cppcomponents/cppcomponents.hpp>
#include "../../cppcomponents_json/cppcomponents_json.hpp"


namespace jrb_json{

	struct value;

  typedef cppcomponents::use<cppcomponents::json::IJsonValue> ivalue;

	typedef std::map<std::string,ivalue > object_type;
  typedef std::vector<ivalue > array_type;
	struct null_t{};
	typedef boost::variant<std::int32_t,std::uint32_t,boost::int64_t,boost::uint64_t,double, std::string,bool,object_type,array_type,null_t> value_holder;

  typedef cppcomponents::error_handle null_error;
	//struct null_error:public std::runtime_error{
	//	null_error():std::runtime_error("Null value"){}
	//};
  typedef cppcomponents::error_invalid_arg invalid_type_error;
  typedef cppcomponents::error_fail json_parse_error;

	//struct invalid_type_error:public std::runtime_error{
	//	invalid_type_error():std::runtime_error("Invalid type"){}
	//};
	//struct json_parse_error:public std::runtime_error{
	//	json_parse_error():std::runtime_error("parse error"){}
	//};




	inline null_t null(){return null_t();}

	struct object_holder{
		object_type& get(){return o_;}
		operator object_type&(){return get();}
		const object_type& get()const{return o_;}
		operator const object_type&()const{return get();}

		object_holder& operator ()(const std::string& name, ivalue d){
			o_[name] = d;
			return *this;
		}
	private:
		object_type o_;

	};

	struct value{

	private:
		//struct array_transform{
		//	ivalue operator()(const array_type::value_type& v)const{
		//		return v;
		//	}
		//};
		//struct object_transform
		//{
		//	const std::pair<const std::string&,const ivalue&> operator() (const object_type::value_type& v)const{
		//		return std::pair<const std::string& ,ivalue>(v.first,v.second);
		//	}

		//};
	public:

		value(){assign(null_t());}

		template<class T>
		value(const T& t){assign(t);}

		value(const char* s){assign(std::string(s));}

		value(const object_holder& v){assign(v);}
		value(null_t (*)()){assign(null_t());}

		value(const value& other){assign(*other.ptr_.get());}
		value(value&& other){ptr_ = std::move(other.ptr_);}

		value& operator=(const value& other){assign(*other.ptr_.get()); return *this;}
		value& operator=(value&& other){ptr_ = std::move(other.ptr_); return *this;}

		value& operator=(null_t (*)()){assign(null_t()); return *this;}

		template<class T>
		T& get_exact(){
			T* ret = boost::get<T>(ptr_.get());
			if(ret){return *ret;}
			else{throw invalid_type_error();}
		}	
		template<class T>
		const T& get_exact()const {
			T* ret = boost::get<T>(ptr_.get());
			if(ret){return *ret;}
			else{throw invalid_type_error();}
		}

		template<class T>
		T get()const{
			get_visitor<T> visitor;
			boost::apply_visitor(visitor,*ptr_);
			return visitor.val_;
		}

		template<class T>
		bool has()const{
			T* ret = boost::get<T>(ptr_.get());
			if(ret) return true;
			else return false;
		}

		ivalue& operator[](int i){
			return get_exact<array_type>().at(i);
		}	
		const ivalue& operator[](int i)const{
			return get_exact<array_type>().at(i);
		}
		ivalue& operator[](const std::string& name){
			return get_exact<object_type>()[name];
		}	
		const ivalue& operator[](const std::string& name)const{
			auto& o = get_exact<object_type>();
			auto iter = o.find(name);
			if(iter != o.end()){
				return iter->second;
			}
			else{
				throw std::out_of_range("value[name] not found");
			}
		}
		typedef array_type::const_iterator array_iterator;
		typedef object_type::const_iterator object_iterator;



		array_iterator array_begin(){return get_exact<array_type>().cbegin();}
		array_iterator array_end(){return get_exact<array_type>().cend();}
		object_iterator object_begin(){return  get_exact<object_type>().cbegin();}
		object_iterator object_end(){return  get_exact<object_type>().cend();}

		void push_back(const ivalue& d)
		{
			get_exact<array_type>().push_back(d);

		}
		int size()const{
			if(has<null_t>()) return 0;
			if(has<array_type>()) return get_exact<array_type>().size();
			if(has<object_type>()) return get_exact<object_type>().size();
			else return 1;
		}

		ivalue& operator[](const char* name){return (*this)[std::string(name)];}
		const ivalue& operator[](const char* name)const{return (*this)[std::string(name)];}



		value_holder& jrb_get_rep(){return *ptr_;}
		const value_holder& jrb_get_rep()const{return *ptr_;}

	private:
		std::unique_ptr<value_holder> ptr_;
		template<class T>
		void assign(const T& t){ptr_.reset(new value_holder(t));}
		void assign(const object_holder& o){ptr_.reset(new value_holder(o.get()));}
		void assign(null_t n){ptr_.reset(new value_holder(null_t()));}




		template<class Type>
		struct get_visitor:public boost::static_visitor<>{

			template<class U>
			void operator()(U t) 
			{

			}	

			void operator()(std::int32_t t) 
			{
				val_ = boost::numeric_cast<Type>(t);
			}	
			void operator()(boost::int64_t t) 
			{
				val_ = boost::numeric_cast<Type>(t);
			}	
			void operator()(std::uint32_t t) 
			{
				val_ = boost::numeric_cast<Type>(t);
			}	
			void operator()(boost::uint64_t t) 
			{
				val_ = boost::numeric_cast<Type>(t);
			}	
			void operator()(double t) 
			{
				val_ = boost::numeric_cast<Type>(t);
			}
			void operator()(bool b)
			{
				val_ = b;
			}
			void operator()(const std::string& str) 
			{
				val_ = boost::lexical_cast<Type>(str);
			}
			void operator()(const object_type&) 
			{
				throw invalid_type_error();
			}
			void operator()(const array_type&) 
			{
				throw invalid_type_error();
			}
			void operator()(const null_t&) 
			{
				throw null_error();
			}
			Type val_;

		};
		template<>
		struct get_visitor<std::string>:public boost::static_visitor<>{

			template<class U>
			void operator()(U t) 
			{

			}	
			void operator()(std::int32_t t) 
			{
				val_ = boost::lexical_cast<std::string>(t);
			}	
			void operator()(boost::int64_t t) 
			{
				val_ = boost::lexical_cast<std::string>(t);
			}	
			void operator()(std::uint32_t t) 
			{
				val_ = boost::lexical_cast<std::string>(t);
			}	
			void operator()(boost::uint64_t t) 
			{
				val_ = boost::lexical_cast<std::string>(t);
			}	
			void operator()(double t) 
			{
				val_ = boost::lexical_cast<std::string>(t);
			}
			void operator()(bool b)
			{
				if(b) val_ = "true";
				else val_ = "false";


			}
			void operator()(const std::string& str) 
			{
				val_ = str;
			}
			void operator()(const object_type&) 
			{
				val_ = "object";
			}
			void operator()(const array_type&) 
			{
				val_ = "array";
			}
			void operator()(const null_t&) 
			{
				val_ = "null";
			}
			std::string val_;

		};





	};

	ivalue parse_json(const std::string& str);

  std::string json_string(ivalue val);
  std::string pretty_json_string(ivalue val);

  inline object_holder object(){
		object_holder ret;
		return ret;
	}



	inline array_type array(){
		return array_type();
	}
}

#endif