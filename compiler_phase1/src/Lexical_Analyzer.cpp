#include "Lexical_Analyzer.h"

Lexical_Analyzer::Lexical_Analyzer(string filename)
{
    // Grammar parsing and construct NFA
    GrammerParser* gr = new GrammerParser("regular_expressions.txt");
    Graph * nfa_graph = gr->getFullNFA();
    vector<string> input = gr->get_weights();
    vector<string> tokens = gr->get_expressions();

    // NFA to DFA
    NFA_To_DFA dfa(nfa_graph, input, tokens);
    vector < vector < DFA_State > > dfa_table = dfa.get_DFA_table();
    vector < DFA_State > dfa_states = dfa.get_DFA_states();

    unordered_map<string, int> acc_tokens_map;
    for(int i=tokens.size()-1; i>=0; i--)
    {
        acc_tokens_map[tokens[i]] = i;
    }

    // Minimization
    Minimization *minimizer = new Minimization(acc_tokens_map);
    vector < vector < DFA_State > > min_dfa = minimizer->minimize_DFA(dfa_table, dfa_states);
    vector < DFA_State > min_states = minimizer->get_minimum_states();

    map<char, int> input_map_table;
    for(int i=0; i<input.size(); i++)
    {
        input_map_table[input[i][0]] = i;
    }

/*  for(auto it = input_map_table.cbegin(); it != input_map_table.cend(); ++it)
    {
        std::cout << it->first << " " << it->second << "\n";
    }
*/

    Tokens *token_manager = new Tokens(min_dfa, input_map_table, min_states, filename);
    string token = token_manager->getToken();
    while(token != ""){
        token = token_manager->getToken();
    }
}
Lexical_Analyzer::~Lexical_Analyzer()
{

}
