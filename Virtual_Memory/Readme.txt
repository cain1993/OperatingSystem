Implementation and Design
Ruiyuan Jiang
The design of this program is listed as follows:
(a)	Let the program read in the logical address and store these addresses into an array. 

(b)	Let the program calculate the page number and the offset according to the logical address. 

(c)	Using the page number to match with the physical memory. Firstly, find it in the TLB block. 
	The TLB block is defined as an array of structure and it contains the access time and the frame number of the page in the memory. 
	If it cannot be found in the TLB, we look it up in the page table. The page table is of the similar structure as the TLB block. 
	And if it can’t be found in page table either. We find it in the backing store, 
	and the program will copy the data from the backing store to the corresponding place in the page table.

(d)	The LRU algorithm is used to replace the page when the physical memory is full(frame pages are all occupied). 
	A global variable WorkTime is set to record the running time of the program. 
	Whenever a page is accessed or required, the access time of the page is updated. 
	The LRU algorithm will find the page with the smallest access time and record it as the least recent used page. 
	It will then be replaced by the incoming one. 

