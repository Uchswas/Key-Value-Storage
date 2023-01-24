## Key Value Storage on Higher Dimentional Data

An Efficient storage system for Higher Dimentional Data .This research work has done as my undergraduate thesis

### Overview
--------

The Generalized Two-dimensional Array (G2A) can be used to represent an n-dimensional (nD) array. By mapping the higher-dimensional data into a 2D array, the index (x, y) of each element of that 2D array is used to generate the corresponding key of the record. After storing the keys in a bin-based file system, we design three query paradigms to search for values.
- Exact Matching
- Single key Query
- Range Key Query



### Build
-------
To create the keyvalues for the dataset, run`create.cpp`

The number of rows for which keyvalues will be generated can be varied by changing the number in line 59 of `create.cpp` file
<pre>
while(fgets(line, sizeof line, fp) != NULL && cnt<10000)
</pre>

Here, the system will create key values for the first 10,000 rows of the `dataset.csv` file and store the key values in different bin files.


#### Exact Matching


Run `exact match.cpp`

Input the values of a row of the csv file to check whether it exists 

##### For instance, to check for the 7th row, input will be 
<pre>
106	   09:04.1	400	  MAGASIN	   11/3/2014    0:00  	 4:18:44  	  PM
</pre>
colunm `id` is excluded as it is a primary key


#### Single key Query 


For Single Key Query, run `singlekeysearch.cpp`

To find the relationships of a single value, enter the column number and one value.
For example, to determine the relationship of data 106 in column 1, the input will be
<pre>
1       106	   
</pre>

#### Range Key Query
----------------------
For Range Key Query, run `rangevaluesearch.cpp`

Only the first column, this feature has been implemented.
Input a range of the values in column 1, like 106 and 110, to find all the relations between the range.
