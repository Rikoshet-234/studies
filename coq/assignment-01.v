(** * Prerequesitses *)
(** Solutions do not depend on any of the _Software Foundations_ source
    files. Instead, standard library definitions are utilized as well as
    simple lemmas provided with them. *)

Require Import Arith.Mult.
Require Import Program.Basics.
Require Import Lists.List.
Require Import Bool.Bool.
Import ListNotations.

Open Scope bool_scope.
Open Scope list_scope.

(* ######################################################################### *)
(** * Problem 1 (10 p.) *)
(** Solve the following exercises: [binary] (Basics), [binary_commute]
    (Induction) and [binary_inverse] (Induction). *)

Inductive bin : Type :=
  | Z : bin
  | T0 : bin -> bin
  | T1 : bin -> bin.

Fixpoint inc_bin (b : bin) : bin :=
  match b with
  | Z     => T1 Z
  | T0 b' => T1 b'
  | T1 b' => T0 (inc_bin b')
  end.

Fixpoint bin_to_nat (b : bin) : nat :=
  match b with
  | Z     => 0
  | T0 b' => 2 * (bin_to_nat b')
  | T1 b' => 1 + 2 * (bin_to_nat b')
  end.

Theorem bin_commute : forall b,
  bin_to_nat (inc_bin b) = S (bin_to_nat b).
Proof.
  intro b.
  induction b as [ | b' | b'].
  
  (* b = 0 *)
  reflexivity.

  (* b = 2b' *)
  reflexivity.

  (* b = 2b' + 1 *)
  simpl.
  rewrite -> plus_0_r.
  rewrite -> plus_0_r.
  rewrite -> IHb'.
  rewrite <- plus_n_Sm.
  rewrite -> plus_Sn_m.
  reflexivity. Qed.

Fixpoint nat_to_bin (n : nat) : bin :=
  match n with
  | O   => Z
  | S n => inc_bin (nat_to_bin n)
  end.

Lemma inc_bin_to_nat : forall b : bin,
  bin_to_nat (inc_bin b) = S (bin_to_nat b).
Proof.
  intro b.
  induction b as [ | b' | b'].

  (* b = 0 *)
  reflexivity.

  (* b = 2 * b' *)
  reflexivity.

  (* b = 2 * b + 1' *)
  simpl.
  rewrite -> plus_0_r.
  rewrite -> plus_0_r.
  rewrite -> IHb'.
  rewrite <- plus_n_Sm.
  rewrite -> plus_Sn_m.
  reflexivity. Qed.

Theorem binary_inverse_nat : forall n : nat,
  bin_to_nat (nat_to_bin n) = n.
Proof.
  intro n.
  induction n as [ | n'].

  (* n = 0 *)
  reflexivity.

  (* n > 0 *)
  simpl.
  rewrite -> inc_bin_to_nat.
  rewrite -> IHn'.
  reflexivity. Qed.

Fixpoint normalize_bin (b : bin) : bin :=
  match b with
  | Z     => Z
  | T0 b' => let normal_b' := normalize_bin b' in
               match normal_b' with
               | Z => Z
               | _ => T0 normal_b'
               end
  | T1 b' => T1 (normalize_bin b')
  end.

Lemma nat_to_bin_double : forall n : nat,
  nat_to_bin (S n + S n) = T0 (nat_to_bin (S n)).
Proof.
  intro n.
  induction n as [ | n'].

  (* n = 0 *)
  reflexivity.

  (* n > 0 *)
  rewrite <- plus_n_Sm.
  rewrite -> plus_Sn_m.
  assert (def_ntb : forall m : nat, nat_to_bin (S m) = inc_bin (nat_to_bin m)).
    reflexivity.
  rewrite -> def_ntb.
  rewrite -> def_ntb.
  rewrite -> IHn'.
  reflexivity. Qed.

Theorem binary_inverse_bin : forall b : bin,
  nat_to_bin (bin_to_nat b) = normalize_bin b.
Proof.
  intro b.
  induction b as [ | b' | b'].

  (* b = 0 *)
  reflexivity.

  (* b = 2 * b' *)
  simpl.
  rewrite -> plus_0_r.
  rewrite <- IHb'.
  destruct (bin_to_nat b').

    (* bin_to_nat b' = 0 *)
    reflexivity.

    (* bin_to_nat b' = S n *)
    rewrite -> nat_to_bin_double.
    simpl.
    destruct (nat_to_bin n) as [ | b'' | b''].
    
      (* nat_to_bin n = Z *)
      reflexivity.

      (* nat_to_bin n = 2 * b'' *)
      reflexivity.

      (* nat_to_bin n = 2 * b'' + 1 *)
      reflexivity.

  (* b = 2 * b' + 1 *)
  simpl.
  rewrite -> plus_0_r.
  destruct (bin_to_nat b').

    (* bin_to_nat b' = 0 *)
    rewrite <- IHb'.
    reflexivity.

    (* bin_to_nat b' = S n *)
    rewrite -> nat_to_bin_double.
    rewrite -> IHb'.
    reflexivity. Qed.

(** [] *)

(* ######################################################################### *)
(** * Problem 2 (5 p.) *)
(** Implement the factorial function in the two usual ways: directly encoding
    the definition of the function, and tail-recursively using an accumulator.
    Prove that the two functions are extensionally equal.  *)

Fixpoint fact_rec (n : nat) : nat :=
  match n with
  | O    => 1
  | S n' => n * fact_rec n'
  end.

Fixpoint fact_aux (acc : nat) (n : nat) : nat :=
  match n with
  | O    => acc
  | S n' => fact_aux (n * acc) n'
  end.

Definition fact_acc := fact_aux 1.

Lemma fact_rec_eq_aux : forall n m : nat,
  m * fact_rec n = fact_aux m n.
Proof.
  intro n.
  induction n as [ | n'].
  
  (* n = 0 *)
  intro m.
  simpl.
  apply mult_1_r.

  (* n > 0 *)
  intro m.
  assert (fr_def : fact_rec (S n') = (S n') * fact_rec n').
    reflexivity.
  rewrite -> fr_def.
  assert (fa_def : fact_aux m (S n') = fact_aux (S n' * m) n').
    reflexivity.
  rewrite -> fa_def.
  rewrite -> mult_assoc.
  assert (swap_acc : m * S n' = S n' * m).
    apply mult_comm.
  rewrite -> swap_acc.
  apply IHn'. Qed.

Theorem fact_rec_eq_acc : forall n : nat,
  fact_rec n = fact_acc n.
Proof.
  intro n.
  unfold fact_acc.
  assert (mult_1 : 1 * fact_rec n = fact_rec n).
    apply mult_1_l.
  rewrite <- mult_1.
  apply fact_rec_eq_aux. Qed.

(** [] *)

(* ######################################################################### *)
(** * Problem 3 (5 p.) *)
(** Solve the exercise [forall_exists_challenge] (MoreCoq). *)

Fixpoint forallb {A : Type} (f : A -> bool) (l : list A) : bool :=
  match l with
  | []      => true
  | x :: l' => (f x) && (forallb f l')
  end.

Fixpoint existsb {A : Type} (f : A -> bool) (l : list A) : bool :=
  match l with
  | []      => false
  | x :: l' => f x || existsb f l'
  end.

Definition existsb' {A : Type} (f : A -> bool) (l : list A) : bool :=
  negb (forallb (compose negb f) l).

Theorem exists_eq : forall {A : Type} (f : A -> bool) (l : list A),
  existsb f l = existsb' f l.
Proof.
  intro A.
  intros f l.
  induction l as [ | x l'].
  
  (* l = [] *)
  reflexivity.

  (* l = x :: l' *)
  simpl.
  unfold existsb'.
  simpl.
  unfold compose at 1.
  rewrite -> negb_andb.
  rewrite -> negb_involutive.
  assert (ex_def : negb (forallb (compose negb f) l') = existsb' f l').
    reflexivity.
  rewrite -> ex_def.
  rewrite <- IHl'.
  reflexivity. Qed.

(** [] *)

(* ######################################################################### *)
(** * Problem 4 (5 p.) *)
(** Prove the third duality law for fold. *)

Definition flip {A B C: Type} (f: A -> B -> C) : B -> A -> C := 
  fun (b : B) => fun (a : A) => f a b.

Fixpoint foldr {A B : Type} (f : A -> B -> B) (u : B) (l : list A) :=
  match l with
  | []      => u
  | x :: l' => f x (foldr f u l')
  end.

Fixpoint foldl {A B : Type} (f : B -> A -> B) (u : B) (l : list A) :=
  match l with
  | []      => u
  | x :: l' => foldl f (f u x) l'
  end.

Lemma foldl_app : forall { A B : Type }
  (f : B -> A -> B) (u : B)
  (l1 : list A) (l2 : list A),
  foldl f u (l1 ++ l2) = foldl f (foldl f u l1) l2.
Proof.
  intros A B.
  intros f u.
  intro l1.
  generalize dependent u.
  induction l1 as [ | x l1'].

  (* l1 = [] *)
  reflexivity.

  (* l1 = x :: l1' *)
  intros u l2.
  simpl.
  apply IHl1'. Qed.

Theorem third_duality_law : forall {A B : Type}
  (f : A -> B -> B) (u : B) (l : list A),
  foldr f u l = foldl (flip f) u (rev l).
Proof.
  intros A B.
  intros f e.
  intro l.
  induction l as [ | x l'].

  (* l = [] *)
  reflexivity.

  (* l = x :: l' *)
  simpl.
  rewrite -> IHl'.
  rewrite -> foldl_app.
  simpl.
  unfold flip.
  reflexivity. Qed.

(** [] *)

(* ######################################################################### *)
(** * Problem 5 (7 p.) *)
(** Prove the following theorems, either constructively, if the theorem is
    constructively valid, or using your favorite classical axiom, if not. *)

Axiom double_negation : forall P : Prop, ~~P -> P.

Theorem t1 : forall P Q R: Prop,
  ((P /\ Q) -> R) <-> (P -> (Q -> R)).
Proof.
  unfold iff.
  intros P Q R.
  apply conj.

  (* left *)
  intros H HP HQ.
  apply H.
  apply conj.
    apply HP.
    apply HQ.

  (* right *)
  intros H HPQ.
  inversion HPQ as [HP HQ].
  apply H.
    apply HP.
    apply HQ. Qed.

Theorem t2 : forall P Q R: Prop,
  (P -> (Q \/ R)) -> (P -> Q) \/ (P -> R).
Proof.
  Abort. (* Proof later. *)

Theorem t3 : forall P Q: Prop,
  ~ (P /\ Q) -> ~P \/ ~Q.
Proof.
  Abort. (* Proof later. *)

Theorem t4 : forall P Q: Prop,
  ~P \/ ~Q -> ~ (P /\ Q).
Proof.
  intros P Q.
  intro H.
  inversion H as [HnotP | HnotQ].

  (* ~P *)
  unfold not.
  intro HPQ.
  inversion HPQ as [HP _].
  unfold not in HnotP.
  apply HnotP in HP.
  apply HP.

  (* ~Q *)
  unfold not.
  intro HPQ.
  inversion HPQ as [_ HQ].
  unfold not in HnotQ.
  apply HnotQ in HQ.
  apply HQ. Qed.

Theorem t4_reverse : forall P Q : Prop,
  ~(P \/ Q) -> ~P /\ ~Q.
Proof.
  intros P Q.
  intro H.
  apply conj.

  (* ~P *)
  unfold not.
  intro HP.
  apply H.
  apply or_introl.
  apply HP.

  (* ~Q *)
  intro HQ.
  apply H.
  apply or_intror.
  apply HQ. Qed.

Theorem t3 : forall P Q: Prop,
  ~ (P /\ Q) -> ~P \/ ~Q.
Proof.
  intros P Q.
  intro H.
  apply double_negation.
  intro notH.
  apply t4_reverse in notH.
  inversion notH as [HP HQ].
  apply double_negation in HP.
  apply double_negation in HQ.
  apply H.
  apply conj.
    apply HP.
    apply HQ. Qed.

Theorem t5 : forall P: Prop,
  (~~P -> P) <-> (P \/ ~P).
Proof.
  intro P.
  apply iff_sym.
  apply conj.

  (* <- *)
  intro H.
  inversion H as [HP | HnotP].

    (* P *)
    unfold not.
    intro impl_PF.
    apply HP.

    (* ~P *)
    unfold not at 1.
    intro impl_notPF.
    apply impl_notPF in HnotP.
    inversion HnotP.

  (* -> *)
  Abort. (* Only proof for stronger version, look below. *)

Theorem t5_strong :
  (forall P : Prop, (~~P -> P)) -> (forall P : Prop, (P \/ ~P)).
Proof.
  intro double_negation.
  intro P.
  apply double_negation.
  unfold not at 1.
  intro H.
  apply t4_reverse in H.
  inversion H as [HnotP HnotnotP].
  apply double_negation in HnotnotP.
  apply HnotP in HnotnotP.
  apply HnotnotP. Qed.

Lemma imply_to_or : forall P Q : Prop, (P -> Q) -> ~P \/ Q.
Proof.
  intros P Q.
  intro H.

  assert (PnotP : P \/ ~P).
    apply t5_strong.
    apply double_negation.
  inversion PnotP as [HP | HnotP].

  (* P *)
  apply or_intror.
  apply H.
  apply HP.

  (* ~P *)
  apply or_introl.
  apply HnotP. Qed.

Theorem t2 : forall P Q R: Prop,
  (P -> (Q \/ R)) -> (P -> Q) \/ (P -> R).
Proof.
  intros P Q R.
  intro H.
  apply imply_to_or in H.

  inversion H as [HnotP | [HQ | HR]].

  (* ~P *)
  apply or_intror.
  intro HP.
  apply HnotP in HP.
  inversion HP.

  (* HQ *)
  apply or_introl.
  intro HP.
  apply HQ.

  (* HR *)
  apply or_intror.
  intro HP.
  apply HR. Qed.

Parameter X : Type.
Parameter P Q : X -> Prop. 

Theorem t6 :
  (forall (x : X), P x /\ Q x) <-> (forall (x : X), P x) /\ (forall (x : X), Q x).
Proof.
  apply conj.
  
  (* -> *)
  intro H.
  apply conj.
    intro x.
    apply H.
    intro x.
    apply H.

  (* <- *)
  intro H.
  intro x.
  apply conj.
    apply H.
    apply H. Qed.

Theorem t7 : (forall (x : X), ~~ P x) -> ~~(forall (x : X), P x).
Proof.
  intro H.
  unfold not.
  intro Hnot.
  apply Hnot.
  intro x.
  apply double_negation.
  apply H. Qed.

(** [] *)

(* ######################################################################### *)
(** * Problem 6 (3 p.) *)
(** Formalize the following reasoning in Coq. Is it constructive or
    classical? *)

(** Figaro shaves all men of Seville who do not shave themselves. 
    Figaro is a man of Seville. 
    Therefore Figaro shaves himself. *)

Parameter man : Type.
Parameter shaves : man -> man -> Prop.
Parameter sevillan : man -> Prop.
Parameter figaro : man.

Axiom figaro_barber : forall m : man,
  sevillan m /\ ~(shaves m m) -> shaves figaro m.
Axiom figaro_sevillan : sevillan figaro.

Theorem russels_paradox : shaves figaro figaro.
Proof.
  apply double_negation.
  intro H.
  
  assert (notH : shaves figaro figaro).
    apply figaro_barber.
    apply conj.
      apply figaro_sevillan.
      apply H.

  unfold not in H.
  apply H in notH.
  apply notH. Qed.

(** [] *)
