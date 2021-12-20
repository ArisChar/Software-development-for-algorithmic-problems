Ανάπτυξη Λογισμικού για Αλγοριθμικά Προβλήματα
2η εργασία: Κατακερματισμός και αναζήτηση για χρονοσειρές

Ομάδα 60:
	Maciej Przestrzelski 1115201600136
	Χαραλάμπης Άρης 	 1115201600192

github link : https://github.com/Aris1600192/Project/tree/main/Project_2/

Για την εργασία υλοποιήθηκαν όλα τα ζητούμενα. 

Το προγραμμα διμιουργεί ένα αρχείο εξόδου.

Η εργασία υλοποιήθηκε με χρήση της c++ με εκτενή χρήση κλάσεων και vector . Για την εκτέλεση των διάφορων ενδεχόμενων εκτέλεσης δημιουργούμε και αντίστοιχα εκτελέσιμα(ένα για LSH ,ένα για Hypercube και ένα για το clustering).  

Τα input και query πρέπει να είναι στο ίδιο φάκελο με το πρόγραμμα 

Τα id των χρονοσειρών τα μετατρέπουμε σε int

Ο Τρόπος εκτέλεσης της μεταγλωττιστής των αρχείων είναι: 

Κάθε φάκελος έχει make file για να τρέξουν οι mains:

Εντολή make για τις main του κάθε ερωτήματος και

εντολή make test για το unit test

Για το Α ερώτημα: 

./main και τα υπόλοιπα arguments είναι:
    -i για το input_file
    -g για το query_file
    -o για το output_file
    -k για το k lsh
    -L για το L lsh
    -K για το k του hypercube
    -M για του hypercube
    -p για probes
    -a για το algorithm
    -m για το metric
    -d για το delta 

Για τον algorithm:
    0 είναι για LSH και το default
    1 είναι για hypercube 
    2 είναι για Frechet
Για το metric:
    0 για L2
    1 για discrete Frechet
    2 για continuous Frechet

Για discrete Frechet
(πχ. ./main -i nasd_input.csv -q nasd_query.csv -o out -a 2 -m 1 ) 

Το ερώτημα Αiii) αργεί αρκετά

Για το Β ερώτημα: 
./cluster_main και τα υπόλοιπα arguments είναι:
	-m θέλει Classic/LSH/ Hypercube 
	-u είναι το update θέλει Frechet ή Vector
	-a είναι το assignment θέλει Classic/LSH/Hypercube/Frechet
	-C είναι το complete θέλει yes or no
	το silhouette πάντα υπολογίχζεται
	
default το πρόγραμμα τρέχει για Classic

./cluster_main και τα υπόλοιπα arguments όπως στην εκφώνηση 

(πχ ./cluster_main -i nasd_input.csv -c cluster.conf -o output_file -u Vector -a Classic -C yes 
	./cluster_main -i nasd_input.csv -c cluster.conf -o output_file -u Vector -a LSH -C yes 
	./cluster_main -i nasd_input.csv -c cluster.conf -o output_file -u Vector -a Hypercube -C yes 
	./cluster_main -i nasd_input.csv -c cluster.conf -o output_file -u Frechet -a Classic -C yes 
	./cluster_main -i nasd_input.csv -c cluster.conf -o output_file -u Frechet -a LSH_Frechet -C yes 
Κατά την ανάπτυξη της εργασίας χρησιμοποιήθηκε το διαδίκτυο για ξεπερασμένα διάφορων δυσκολιών. 

Στο config file δεν πρέπει να υπάρχει κενό μετά (:) 

Unit test κάναμε σε 2 συναρτήσεις στο A ερώτημα (modulo και euc_dist)