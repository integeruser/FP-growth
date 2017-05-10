#ifndef FPTREE_HPP
#define FPTREE_HPP

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include <utility>


using Item = std::string;
using Transaction = std::vector<Item>;
using TransformedPrefixPath = std::pair<std::vector<Item>, unsigned>;
using Pattern = std::pair<std::set<Item>, unsigned>;


struct FPNode {
    const Item item;
    unsigned frequency;
    std::shared_ptr<FPNode> node_link;
    std::shared_ptr<FPNode> parent;
    std::vector<std::shared_ptr<FPNode>> children;
    
    FPNode(const Item&, const std::shared_ptr<FPNode>&);
};



struct FPTree {
    std::shared_ptr<FPNode> root;
    std::map<Item, std::shared_ptr<FPNode>> header_table;
    unsigned minimum_support_treshold;
    
    FPTree(const std::vector<Transaction>&, unsigned);
    
    bool empty() const;
};


std::set<Pattern> fptree_growth(const FPTree&);


#endif  // FPTREE_HPP
