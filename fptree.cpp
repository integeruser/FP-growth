#include <algorithm>
#include <cassert>
#include <utility>

#include "fptree.hpp"


FPNode::FPNode(const Item& item, const std::shared_ptr<FPNode>& parent) :
    item( item ), frequency( 1 ), node_link( nullptr ), parent( parent ), children()
{
}



FPTree::FPTree(const std::vector<Transaction>& transactions, unsigned minimum_support_treshold) :
    root( nullptr ), header_table(), minimum_support_treshold( minimum_support_treshold )
{
    // scan the transactions counting the frequence of each item
    std::map<Item, unsigned> frequency_by_item;
    for ( const Transaction& transaction : transactions ) {
        for ( const Item& item : transaction ) {
            ++frequency_by_item[item];
        }
    }
    
    // keep only items which have a frequency greater or equal than the minimum support treshold
    for ( auto it = frequency_by_item.cbegin(); it != frequency_by_item.cend(); ) {
        const unsigned item_frequency = (*it).second;
        if ( item_frequency < minimum_support_treshold ) { frequency_by_item.erase( it++ ); }
        else { ++it; }
    }
    
    // order items by decreasing frequency
    struct decreasing_order_comparator {
        bool operator() (const std::pair<unsigned, Item>& lhs, const std::pair<unsigned, Item>& rhs) const {
            return (lhs.first > rhs.first) || (!(lhs.first > rhs.first) && lhs.second < rhs.second);
        }
    };
    std::set<std::pair<unsigned, Item>, decreasing_order_comparator> items_ordered_by_frequency;
    for ( const auto& pair : frequency_by_item ) {
        const Item& item = pair.first;
        const unsigned frequency = pair.second;
        items_ordered_by_frequency.insert( { frequency, item } );
    }

    // start tree construction
    root = std::make_shared<FPNode>( Item{}, nullptr );
    
    // scan the transactions again
    for ( const Transaction& transaction : transactions ) {
        std::shared_ptr<FPNode> curr_fpnode = root;

        // select and sort the frequent items in transaction according to the order of items_ordered_by_frequency
        for ( const auto& pair : items_ordered_by_frequency ) {
            const Item& item = pair.second;
            // check if item is contained in the current transaction
            if ( std::find( transaction.cbegin(), transaction.cend(), item ) != transaction.cend() ) {
                // insert item in the tree
            
                // check if curr_fpnode has a child curr_fpnode_child such that curr_fpnode_child.item = item
                const auto it = std::find_if ( curr_fpnode->children.cbegin(), curr_fpnode->children.cend(), [item](const std::shared_ptr<FPNode>& fpnode) {
                    return fpnode->item == item;
                } );
                if ( it == curr_fpnode->children.cend() ) {
                    // the child doesn't exist, create a new node
                    const std::shared_ptr<FPNode> curr_fpnode_new_child = std::make_shared<FPNode>( item, curr_fpnode );
                    
                    // add the new node to the tree
                    curr_fpnode->children.push_back( curr_fpnode_new_child );
                    
                    // update the node-link structure
                    if ( header_table.count( curr_fpnode_new_child->item ) ) {
                        std::shared_ptr<FPNode> prev_fpnode = header_table[curr_fpnode_new_child->item];
                        while ( prev_fpnode->node_link ) { prev_fpnode = prev_fpnode->node_link; }
                        prev_fpnode->node_link = curr_fpnode_new_child;
                    }
                    else {
                        header_table[curr_fpnode_new_child->item] = curr_fpnode_new_child;
                    }
                    
                    // advance to the next node of the current transaction
                    curr_fpnode = curr_fpnode_new_child;
                }
                else {
                    // the child exist, increment its frequency
                    std::shared_ptr<FPNode> curr_fpnode_child = *it;
                    ++curr_fpnode_child->frequency;
                    
                    // advance to the next node of the current transaction
                    curr_fpnode = curr_fpnode_child;
                }
            }
        }
    }
}


bool contains_single_path(const std::shared_ptr<FPNode>& fpnode) {
    if ( !fpnode || fpnode->children.size() == 0 ) { return true; }
    if ( fpnode->children.size() > 1 ) { return false; }
    return contains_single_path( fpnode->children.front() );
}
bool contains_single_path(const FPTree& fptree) {
    return contains_single_path( fptree.root );
}
                          
std::set<Pattern> fptree_growth(const FPTree& fptree) {
    if ( fptree.empty() ) { return std::set<Pattern>(); }
    
    if ( contains_single_path( fptree ) ) {
        // generate all possible combinations of the items in the tree
        
        std::set<Pattern> single_path_patterns;
        
        // for each node in the tree
        assert( fptree.root->children.size() == 1 );
        std::shared_ptr<FPNode> curr_fpnode = fptree.root->children.front();
        while ( curr_fpnode ) {
            const Item& curr_fpnode_item = curr_fpnode->item;

            // add a pattern formed only by the item of the current node
            Pattern new_pattern = { curr_fpnode_item };
            single_path_patterns.insert( new_pattern );
            
            // create a new pattern by adding the item of the current node to each pattern generated until now
            for ( const Pattern& pattern : single_path_patterns ) {
                Pattern new_pattern{ pattern };
                new_pattern.insert( curr_fpnode_item );
                
                single_path_patterns.insert( new_pattern );
            }

            // advance to the next node until the end of the tree
            assert( curr_fpnode->children.size() <= 1 );
            if ( curr_fpnode->children.size() == 1 ) { curr_fpnode = curr_fpnode->children.front(); }
            else { curr_fpnode = nullptr; }
        }
        
        return single_path_patterns;
    }
    else {
        // generate conditional fptrees for each different item in the fptree, then join the results

        std::set<Pattern> multi_path_patterns;
        
        // for each item in the fptree
        for ( const auto& pair : fptree.header_table ) {
            const Item& curr_item = pair.first;
            
            // build the conditional fptree relative to the current item
            
            // start by generating the conditional pattern base
            std::vector<TransformedPrefixPath> conditional_pattern_base;
            
            // for each path in the header_table (relative to the current item)
            std::shared_ptr<FPNode> path_starting_fpnode = pair.second;
            while ( path_starting_fpnode ) {
                // construct the transformed prefix path
                
                // each item in th transformed prefix path has the same frequency (the frequency of path_starting_fpnode)
                const unsigned path_starting_fpnode_frequency = path_starting_fpnode->frequency;

                std::shared_ptr<FPNode> curr_path_fpnode = path_starting_fpnode->parent;
                // check if curr_path_fpnode is already the root of the fptree
                if ( curr_path_fpnode->parent ) {
                    // the path has at least one node (excluding the starting node and the root)
                    TransformedPrefixPath transformed_prefix_path{ {}, path_starting_fpnode_frequency };
                    
                    while ( curr_path_fpnode->parent ) {
                        assert( curr_path_fpnode->frequency >= path_starting_fpnode_frequency );
                        transformed_prefix_path.first.push_back( curr_path_fpnode->item );
                        
                        // advance to the next node in the path
                        curr_path_fpnode = curr_path_fpnode->parent;
                    }
                    
                    conditional_pattern_base.push_back( transformed_prefix_path );
                }
                
                // advance to the next path
                path_starting_fpnode = path_starting_fpnode->node_link;
            }
        
            // generate the transactions that represent the conditional pattern base
            std::vector<Transaction> conditional_fptree_transactions;
            for ( const TransformedPrefixPath& transformed_prefix_path : conditional_pattern_base ) {
                const std::vector<Item>& transformed_prefix_path_items = transformed_prefix_path.first;
                const unsigned transformed_prefix_path_items_frequency = transformed_prefix_path.second;
                
                Transaction transaction;
                for ( const Item& item : transformed_prefix_path_items ) { transaction.push_back( item ); }
                
                // add the same transaction transformed_prefix_path_items_frequency times
                for ( int i = 0; i < transformed_prefix_path_items_frequency; ++i ) { conditional_fptree_transactions.push_back( transaction ); }
            }
            
            // build the conditional fptree relative to the current item with the transactions just generated
            const FPTree conditional_fptree( conditional_fptree_transactions, fptree.minimum_support_treshold );
            // call recursively fptree_growth on the conditional fptree (empty fptree: no patterns)
            std::set<Pattern> conditional_patterns = fptree_growth( conditional_fptree );
            
            // construct patterns relative to the current item using the conditional patterns
            std::set<Pattern> curr_item_patterns;
            curr_item_patterns.insert( { curr_item } );
            // add the current item to each conditional pattern
            for ( const Pattern& pattern : conditional_patterns ) {
                Pattern new_pattern{ pattern };
                new_pattern.insert( curr_item );
                
                curr_item_patterns.insert( { new_pattern } );
            }
            
            // join the patterns generated by the current item with all the other items of the fptree
            multi_path_patterns.insert( curr_item_patterns.cbegin(), curr_item_patterns.cend() );
        }
        
        return multi_path_patterns;
    }
}
