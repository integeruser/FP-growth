#include <cassert>
#include <iostream>
#include <set>
#include <vector>

#include "fptree.hpp"


void test_1() {
    const Item a{ "A" };
    const Item b{ "B" };
    const Item c{ "C" };
    const Item d{ "D" };
    const Item e{ "E" };
    
    const std::vector<Transaction> transactions{
        { a, b },
        { b, c, d },
        { a, c, d, e },
        { a, d, e },
        { a, b, c },
        { a, b, c, d },
        { a },
        { a, b, c },
        { a, b, d },
        { b, c, e }
    };
    
    const unsigned minimum_support_treshold = 2;
    
    const FPTree fptree{ transactions, minimum_support_treshold };
    
    const std::set<Pattern> patterns = fptree_growth( fptree );
    
    assert( patterns.size() == 19 );
    assert( patterns.count( { a } ) );
    assert( patterns.count( { b, a } ) );
    assert( patterns.count( { b } ) );
    assert( patterns.count( { c, b } ) );
    assert( patterns.count( { c, a, b } ) );
    assert( patterns.count( { c, a } ) );
    assert( patterns.count( { c } ) );
    assert( patterns.count( { d, a } ) );
    assert( patterns.count( { d, c, a } ) );
    assert( patterns.count( { d, c } ) );
    assert( patterns.count( { d, b, a } ) );
    assert( patterns.count( { d, b, c } ) );
    assert( patterns.count( { d, b } ) );
    assert( patterns.count( { d } ) );
    assert( patterns.count( { e, d } ) );
    assert( patterns.count( { e, c } ) );
    assert( patterns.count( { e, a, d } ) );
    assert( patterns.count( { e, a } ) );
    assert( patterns.count( { e } ) );
}

void test_2() {
    const Item a{ "A" };
    const Item b{ "B" };
    const Item c{ "C" };
    const Item d{ "D" };
    const Item e{ "E" };
    
    const std::vector<Transaction> transactions{
        { a, b, d, e },
        { b, c, e },
        { a, b, d, e },
        { a, b, c, e },
        { a, b, c, d, e },
        { b, c, d },
    };
    
    const unsigned minimum_support_treshold = 3;
    
    const FPTree fptree{ transactions, minimum_support_treshold };
    
    const std::set<Pattern> patterns = fptree_growth( fptree );
    
    assert( patterns.size() == 19 );
    assert( patterns.count( { e, b } ) );
    assert( patterns.count( { e } ) );
    assert( patterns.count( { a, b, e } ) );
    assert( patterns.count( { a, b } ) );
    assert( patterns.count( { a, e } ) );
    assert( patterns.count( { a } ) );
    assert( patterns.count( { d, a, b } ) );
    assert( patterns.count( { d, a } ) );
    assert( patterns.count( { d, e, b, a } ) );
    assert( patterns.count( { d, e, b } ) );
    assert( patterns.count( { d, e, a } ) );
    assert( patterns.count( { d, e } ) );
    assert( patterns.count( { d, b } ) );
    assert( patterns.count( { d } ) );
    assert( patterns.count( { c, e, b } ) );
    assert( patterns.count( { c, e } ) );
    assert( patterns.count( { c, b } ) );
    assert( patterns.count( { c } ) );
    assert( patterns.count( { b } ) );
}

void test_3() {
    const Item a{ "A" };
    const Item b{ "B" };
    const Item c{ "C" };
    const Item d{ "D" };
    const Item e{ "E" };
    const Item f{ "F" };
    const Item g{ "G" };
    const Item h{ "H" };
    const Item i{ "I" };
    const Item j{ "J" };
    const Item k{ "K" };
    const Item l{ "L" };
    const Item m{ "M" };
    const Item n{ "N" };
    const Item o{ "O" };
    const Item p{ "P" };
    const Item s{ "S" };
    
    const std::vector<Transaction> transactions{
        { f, a, c, d, g, i, m, p },
        { a, b, c, f, l, m, o },
        { b, f, h, j, o },
        { b, c, k, s, p },
        { a, f, c, e, l, p, m, n }
    };
    
    const unsigned minimum_support_treshold = 3;

    
    const FPTree fptree{ transactions, minimum_support_treshold };
    
    const std::set<Pattern> patterns = fptree_growth( fptree );
    
    assert( patterns.size() == 18 );
    assert( patterns.count( { f } ) );
    assert( patterns.count( { c, f } ) );
    assert( patterns.count( { c } ) );
    assert( patterns.count( { b } ) );
    assert( patterns.count( { p, c } ) );
    assert( patterns.count( { p } ) );
    assert( patterns.count( { m, f, c } ) );
    assert( patterns.count( { m, f } ) );
    assert( patterns.count( { m, c } ) );
    assert( patterns.count( { m } ) );
    assert( patterns.count( { a, f, c, m } ) );
    assert( patterns.count( { a, f, c } ) );
    assert( patterns.count( { a, f, m } ) );
    assert( patterns.count( { a, f } ) );
    assert( patterns.count( { a, c, m } ) );
    assert( patterns.count( { a, c } ) );
    assert( patterns.count( { a, m } ) );
    assert( patterns.count( { a } ) );
}

int main(int argc, const char *argv[]) {
    test_1();
    test_2();
    test_3();
    std::cout << "All tests passed!" << std::endl;
    
    return EXIT_SUCCESS;
}
