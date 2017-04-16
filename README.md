# command line:
* **clone server+client:**  
`git clone --recursive git@github.com:vinc96/cs3505_project2.git cs3505_project2` **or** `git clone git@github.com:vinc96/cs3505_project2.git cs3505_project2 && cd cs3505_project2 && git submodule init && git submodule update`  

* **init client as submodule (at cs3505_project2 root):**  
`git submodule init`  

* **update client (at cs3505_project2 root):**  
`git submodule update` **or** `cd client && git pull`  

* **check submodule status:**  
`git submodule status`
