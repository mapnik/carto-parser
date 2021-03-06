/*==============================================================================
    Copyright (c) 2010 Colin Rundel

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file BOOST_LICENSE_1_0.rst or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef MSS_PARSER_H
#define MSS_PARSER_H

#include <parse/parse_tree.hpp>

#include <utility/utree.hpp>
#include <utility/environment.hpp>

#include <boost/variant.hpp>
#include <boost/unordered_map.hpp>

#include <mapnik/rule.hpp>
#include <mapnik/expression_grammar.hpp>
#include <mapnik/map.hpp>
#include <mapnik/noncopyable.hpp>


namespace carto {

struct mss_parser : private mapnik::noncopyable {

private:
    parse_tree tree;
    bool strict;
    std::string path;
    
    boost::unordered_map<std::size_t, std::string> fontset_names;
    mapnik::transcoder tr_;
    mapnik::expression_grammar<std::string::const_iterator> expr_grammar;
    
public:
    mss_parser(parse_tree const& pt, std::string const& path_, bool strict_ = false);  
    mss_parser(std::string const& in, std::string const& path_, bool strict_ = false);
    mss_parser(std::string const& filename, bool strict_ = false);

    std::string const& get_fontset_name(std::size_t hash);
    
    parse_tree get_parse_tree();
    std::string get_path();

    int get_node_type(utree const& ut);    
    source_location get_location(utree const& ut);

    template<class symbolizer>
    symbolizer* find_symbolizer(mapnik::rule& rule) 
    {
        typedef mapnik::rule::symbolizers::iterator iter;
        iter it  = rule.begin(),
             end = rule.end();
        
        for(; it!=end; ++it) {
            if (symbolizer *sym = boost::get<symbolizer>(&(*it)))
                return(sym);
        }
        
        BOOST_ASSERT(it==end);
        rule.append(init_symbolizer<symbolizer>());
        
        symbolizer *sym = boost::get<symbolizer>(& (*(--rule.end())) );
        return(sym);
    }
    
    template<class symbolizer>
    symbolizer init_symbolizer() 
    {
        return symbolizer();
    }

    mapnik::transform_type create_transform(std::string const& str, utree const& node);
    
    void key_error(std::string const& key, utree const& node);
    
    utree eval_var(utree const& node, style_env const& env);    
    utree parse_value(utree const& node, style_env const& env);
    void parse_variable(utree const& node, style_env& env);

    void parse(mapnik::Map& map, style_env& env);
    void parse_stylesheet(mapnik::Map& map, style_env& env);
    void parse_map_style(mapnik::Map& map, utree const& node, style_env& env);
    void parse_style(mapnik::Map& map, utree const& node, style_env const& parent_env, 
                     mapnik::rule const& parent_rule = mapnik::rule(), std::string const& parent_name = "");
    void parse_filter(mapnik::Map& map, utree const& node, style_env const& env, mapnik::rule& rule);
    void parse_attribute(mapnik::Map& map, utree const& node, style_env const& env, mapnik::rule& rule);
    
    bool parse_polygon(mapnik::rule& rule, std::string const& key, utree const& value, style_env const& env);
    bool parse_line(mapnik::rule& rule, std::string const& key, utree const& value, style_env const& env);
    bool parse_marker(mapnik::rule& rule, std::string const& key, utree const& value, style_env const& env);
    bool parse_point(mapnik::rule& rule, std::string const& key, utree const& value, style_env const& env);
    bool parse_line_pattern(mapnik::rule& rule, std::string const& key, utree const& value, style_env const& env);
    bool parse_polygon_pattern(mapnik::rule& rule, std::string const& key, utree const& value, style_env const& env);
    bool parse_raster(mapnik::rule& rule, std::string const& key, utree const& value, style_env const& env);
    bool parse_building(mapnik::rule& rule, std::string const& key, utree const& value, style_env const& env);
    bool parse_text(mapnik::Map& map, mapnik::rule& rule, std::string const& key, utree const& value, style_env const& env);
    bool parse_shield(mapnik::rule& rule, std::string const& key, utree const& value, style_env const& env);    
};

}
#endif 
