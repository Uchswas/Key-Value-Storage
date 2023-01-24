# Key Value Storage on Higher Dimentional Data

An Efficient storage system for Higher Dimentional Data 

This research work has done as my undergraduate thesis

Overview
--------

The Generalized Two-dimensional Array (G2A) can be used to represent an n-dimensional (nD) array.By mapping the higher dimenntional data into a 2D array, the index(x,y) of each element of that 2D array is used to generated the corospoding key of the record.After storing the keys in a bin-based file system, we design three query paradigms to search values 

- Exact Matching
- Single key Query
- Range Key Query



Build
-------
To create the keyvalue of the dataset run `create.cpp`

Number of rows for which keyvalue will be generated can be varied by changing the number in 59 line of `create.cpp` file   
<pre>
while(fgets(line, sizeof line, fp) != NULL && cnt<10000)
</pre>

Here the system will create keyvalues for the first 10000 rows of the `dataset.csv` file and store the key values in differnt bin files


Exact Matching
-------

Run `exact match.cpp`

Input the values of a row of the csv file to check whether it exists 

##### For instance, to check for the 7th row, input will be 
<pre>
106	   09:04.1	400	  MAGASIN	   11/3/2014    0:00  	 4:18:44  	  PM
</pre>
colunm `id` is excluded as it is a primary key


Single key Query 
----------------------

Run `singlekeysearch.cpp`  for Single Key Query.

Input the column number and one value to find the relationships of the single value
##### For instance, to find the relation of data 106 in colunm 1, input will be 

<pre>
1       106	   
</pre>

Range Key Query
----------------------

Implemented only for the column 1 .
Input a range of the values of column 1 like 106 and 110 to find all the relations between the range.