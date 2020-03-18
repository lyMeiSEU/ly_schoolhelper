load
infile 'd:\st.csv'
into table st1
fields terminated  by','
(name char, subject char,score integer external)