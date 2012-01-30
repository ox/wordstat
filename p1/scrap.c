hash_element *tmp = new_hash_element("alpha");
hash_table_store(table, tmp->str, tmp);
free(tmp);

hash_element_add_occurance(hash_table_get(table, "alpha"), "alpha");
hash_element_add_occurance(hash_table_get(table, "alpha"), "alphA");

tmp = new_hash_element("hello");
hash_table_store(table, tmp->str, tmp);
free(tmp);

hash_element_add_occurance(hash_table_get(table, "hello"), "hello");
hash_element_add_occurance(hash_table_get(table, "hello"), "hellO");
hash_element_add_occurance(hash_table_get(table, "hello"), "hEllO");
hash_element_add_occurance(hash_table_get(table, "hello"), "hellO");

tmp = new_hash_element("bye");
hash_table_store(table, tmp->str, tmp);
free(tmp);

hash_element_add_occurance(hash_table_get(table, "bye"), "bye");
hash_element_add_occurance(hash_table_get(table, "bye"), "byE");

tmp = new_hash_element("iamareallylon");
hash_table_store(table, tmp->str, tmp);
free(tmp);

Some Bogus ?Random1> (random1modnar) [34ranDom1] (Random1) boGus-$con@tent.