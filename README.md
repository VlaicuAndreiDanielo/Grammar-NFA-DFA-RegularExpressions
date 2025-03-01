# Grammar to NFA to DFA  

## 📌 Overview  
This project focuses on transforming a **regular expression** into different representations, ultimately constructing a **Deterministic Finite Automaton (DFA)**. The process follows these steps:  

1. **Regular Expression Processing**  
   - The regular expression is read from a file and converted into its **postfix notation**.  

2. **Grammar and Nondeterministic Finite Automaton (NFA) Generation**  
   - The **postfix notation** is used to construct a **grammar**.  
   - From the grammar, a **Nondeterministic Finite Automaton (NFA) with lambda (ε) transitions** is generated.  

3. **Conversion to Deterministic Finite Automaton (DFA)**  
   - The **NFA with lambda transitions** is transformed into a **DFA**.  
   - The DFA allows a **word to be processed through the automaton**, verifying whether it is accepted or rejected.  

4. **Tabular Representation of State Transitions**  
   - A **state transition table** is generated, showing how each state transitions to another based on input symbols.  

## 🛠️ Features  
- **Automated transformation** of a regular expression into DFA.  
- **Tabular representation** of state transitions for better clarity.   
- **Enhanced text rendering and display formatting**, using Unicode.

