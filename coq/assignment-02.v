(** * Prerequesitses *)
Require Export Equiv.
Require Export Hoare2.

(* ######################################################################### *)
(** * Problem 1 (10 p.) *)
(** Solve the following exercises:
        - <<no_whiles>> ([Imp]),
        - <<no_whiles_terminating>> ([Imp]). *)

Inductive no_whilesR : com -> Prop :=
  | NW_Skip : no_whilesR SKIP
  | NW_Ass : forall (X : id) (a : aexp),
    no_whilesR (X ::= a)
  | NW_Seq : forall (c1 c2 : com),
    no_whilesR c1 ->
    no_whilesR c2 ->
    no_whilesR (c1 ;; c2)
  | NW_If : forall (b : bexp) (c1 c2 : com),
    no_whilesR c1 ->
    no_whilesR c2 ->
    no_whilesR (IFB b THEN c1 ELSE c2 FI).

Theorem no_whiles_eqv : forall (c : com),
  no_whiles c = true <-> no_whilesR c.
Proof.
  intro c.
  split;
  intro H.

  (* -> *)
  induction c;

    (* SKIP and i := a *)
    try constructor;

    (* c1 ;; c2 and IFB b THEN c1 else c2 *)
    inversion H;
    apply andb_true in H1;
    inversion H1 as [Hc1 Hc2];
    try (apply IHc1);
    try (apply IHc2);
    assumption.

  (* <- *)
  induction H;

    (* SKIP and i := a *)
    try reflexivity;

    (* c1 ;; c2 and IFB b THEN c1 else c2 *)
    simpl;
    rewrite -> IHno_whilesR1;
    rewrite -> IHno_whilesR2;
    reflexivity. Qed.

Definition terminates (c : com) (st : state) : Prop :=
  exists (st' : state), c / st || st'.

Theorem no_whiles_terminating : forall (c : com),
  no_whilesR c -> forall (st : state), terminates c st.
Proof.
  intros c.
  intro H.
  induction H;
  intro st;
  unfold terminates.

  (* SKIP *)
  exists st.
  apply E_Skip.

  (* X := a *)
  exists (update st X (aeval st a)).
  apply E_Ass.
  reflexivity.

  (* c1 ;; c2 *)
  assert (Hc1 : terminates c1 st).
    apply IHno_whilesR1.
  inversion Hc1 as [st'].
  assert (Hc2 : terminates c2 st').
    apply IHno_whilesR2.
  inversion Hc2 as [st''].
  exists st''.
  apply E_Seq with (st' := st').
  assumption.
  assumption.

  (* IFB b THEN c1 ELSE c2 *)
  remember (beval st b) as b'.
  symmetry in Heqb'.
  destruct b'.
    (* beval st b = true *)
    assert (Hc1 : terminates c1 st).
      apply IHno_whilesR1.
    inversion Hc1 as [st'].
    exists st'.
    apply E_IfTrue.
    assumption.
    assumption.

    (* beval st = false *)
    assert (Hc2 : terminates c2 st).
      apply IHno_whilesR2.
    inversion Hc2 as [st'].
    exists st'.
    apply E_IfFalse.
    assumption.
    assumption. Qed.

(* ######################################################################### *)
(** * Problem 2 (15 p.) *)
(** Solve the following exercises:
        - <<break_imp>> ([Imp]),
        - <<while_break_true>> ([Imp]),
        - <<ceval_deterministic>> ([Imp]). *)

Module BreakImp.

(** ** Definitions *)
(** Since original definitions imported from [Imp] module are enclosed with
    extra namespace and I have no idea how to reuse this namespace I'm pasting
    some of them here again. *)

Inductive com : Type :=
  | CSkip : com
  | CBreak : com
  | CAss : id -> aexp -> com
  | CSeq : com -> com -> com
  | CIf : bexp -> com -> com -> com
  | CWhile : bexp -> com -> com.

Notation "'SKIP'" :=
  CSkip.
Notation "'BREAK'" :=
  CBreak.
Notation "x '::=' a" :=
  (CAss x a) (at level 60).
Notation "c1 ; c2" :=
  (CSeq c1 c2) (at level 80, right associativity).
Notation "'WHILE' b 'DO' c 'END'" :=
  (CWhile b c) (at level 80, right associativity).
Notation "'IFB' c1 'THEN' c2 'ELSE' c3 'FI'" :=
  (CIf c1 c2 c3) (at level 80, right associativity).

Inductive status : Type :=
  | SContinue : status
  | SBreak : status.

Reserved Notation "c1 '/' st '||' s '/' st'"
                  (at level 40, st, s at level 39).

Inductive ceval : com -> state -> status -> state -> Prop :=
  | E_Skip : forall (st : state),
    SKIP / st || SContinue / st
  | E_Break : forall (st : state),
    BREAK / st || SBreak / st
  | E_Ass : forall (st : state) (n : nat) (i : id) (a : aexp),
    aeval st a = n ->
    (i ::= a) / st || SContinue / (update st i n)
  | E_SeqCont : forall (st st' st'' : state) (s : status) (c1 c2 : com),
    c1 / st || SContinue / st' ->
    c2 / st' || s / st'' ->
    (c1 ; c2) / st || s / st''
  | E_SeqBreak : forall (st st' : state) (c1 c2 : com),
    c1 / st || SBreak / st' ->
    (c1 ; c2) / st || SBreak / st'
  | E_IfTrue : forall (st st' : state) (s : status) (b : bexp) (c1 c2 : com),
    beval st b = true ->
    c1 / st || s / st' ->
    IFB b THEN c1 ELSE c2 FI / st || s / st'
  | E_IfFalse : forall (st st' : state) (s : status) (b : bexp) (c1 c2 : com),
    beval st b = false ->
    c2 / st || s / st' ->
    IFB b THEN c1 ELSE c2 FI / st || s / st'
  | E_WhileEnd : forall (st : state) (b : bexp) (c : com),
    beval st b = false ->
    WHILE b DO c END / st || SContinue / st
  | E_WhileBreak : forall (st st' : state) (b : bexp) (c : com),
    beval st b = true ->
    c / st || SBreak / st' ->
    WHILE b DO c END / st || SContinue / st'
  | E_WhileLoop : forall (st st' st'' : state) (b : bexp) (c : com),
    beval st b = true ->
    c / st || SContinue / st' ->
    WHILE b DO c END / st' || SContinue / st'' ->
    WHILE b DO c END / st || SContinue / st''
  where "c1 '/' st '||' s '/' st'" := (ceval c1 st s st').

(** ** Properties *)

Theorem break_ignore : forall c st st' s,
  (BREAK; c) / st || s / st' ->
  st = st'.
Proof.
  intro c.
  intros st st' s.
  intro H.
  inversion H. subst.
  inversion H2. subst.
  inversion H5. subst.
  reflexivity. Qed.

Theorem while_continue : forall b c st st' s,
  (WHILE b DO c END) / st || s / st' ->
  s = SContinue.
Proof.
  intros b c.
  intros st st' s.
  intro H.
  inversion H; subst;
  reflexivity. Qed.

Theorem while_stops_on_break : forall b c st st',
  beval st b = true ->
  c / st || SBreak / st' ->
  (WHILE b DO c END) / st || SContinue / st'.
Proof.
  intros b c.
  intros st st'.
  intros Hb H.
  apply E_WhileBreak.
  assumption.
  assumption. Qed.

Theorem while_break_true : forall b c st st',
  (WHILE b DO c END) / st || SContinue / st' ->
  beval st' b = true ->
  exists st'', c / st'' || SBreak / st'.
Proof.
  intros b c.
  intros st st'.
  intros H Hb.

  remember (WHILE b DO c END) as c'.
  induction H;
  inversion Heqc'; subst.

  (* E_WhileEnd *)
  rewrite -> Hb in H.
  inversion H.

  (* E_WhileBreak *)
  exists st.
  assumption.

  (* E_WhileLoop *)
  apply IHceval2.
  reflexivity.
  assumption. Qed.

(** ** Determinism *)

Theorem ceval_deterministic : forall c st st1 st2 s1 s2,
  c / st || s1 / st1  ->
  c / st || s2 / st2 ->
  st1 = st2 /\ s1 = s2.
Proof.
  intro c.
  intros st st1 st2 s1 s2.
  intros H1 H2.
  generalize dependent st2.
  generalize dependent s2.
  induction H1; intros s2 st2;
  intro H2;
  inversion H2; subst;

  (* E_Skip, E_Break, E_Ass, E_WhileEnd *)
  try (split; reflexivity).

  (* E_SeqCont *)
  assert (Hsteqst' : st' = st'0 /\ SContinue = SContinue).
    apply IHceval1.
    assumption.
  inversion Hsteqst'. subst.
  apply IHceval2.
  assumption.

  (* E_SeqBreak *)
  apply IHceval1 in H5.
  inversion H5.
  inversion H0.

  apply IHceval in H3.
  inversion H3.
  inversion H0.

  apply IHceval in H6.
  assumption.

  (* E_IfTrue *)
  apply IHceval in H9.
  assumption.

  (* E_IfFalse *)
  rewrite -> H in H8.
  inversion H8.

  rewrite -> H in H8.
  inversion H8.

  apply IHceval in H9.
  assumption.

  (* E_WhileBreak *)
  rewrite -> H in H3.
  inversion H3.

  rewrite -> H in H3.
  inversion H3.

  rewrite -> H in H7.
  inversion H7.

  apply IHceval in H8.
  inversion H8.
  split.
  assumption.
  reflexivity.

  apply IHceval in H5.
  inversion H5.
  inversion H3.

  (* E_WhileLoop *)
  rewrite -> H in H6.
  inversion H6.

  apply IHceval1 in H7.
  inversion H7.
  inversion H1.

  assert (Hsteqst' : st' = st'0 /\ SContinue = SContinue).
    apply IHceval1.
    assumption.
  inversion Hsteqst'. subst.
  apply IHceval2.
  assumption. Qed.

End BreakImp.

(* ######################################################################### *)
(** * Problem 3 (20 p.) *)
(** Solve the following exercises:
        - <<himp_ceval>> ([Equiv]),
        - <<havoc_swap>> ([Equiv]),
        - <<havoc_copy>> ([Equiv]),
        - <<p1_p2_equiv>> ([Equiv]),
        - <<p3_p4_equiv>> ([Equiv]),
        - <<p5_p6_equiv>> ([Equiv]). *)

Module Himp.

(** ** Definitions *)
(** Similarly to the previous exercise, required definitions are copied from
    the [Equiv] module. *)

Inductive com : Type :=
  | CSkip : com
  | CAss : id -> aexp -> com
  | CSeq : com -> com -> com
  | CIf : bexp -> com -> com -> com
  | CWhile : bexp -> com -> com
  | CHavoc : id -> com.

Notation "'SKIP'" :=
  CSkip.
Notation "X '::=' a" :=
  (CAss X a) (at level 60).
Notation "c1 ;; c2" :=
  (CSeq c1 c2) (at level 80, right associativity).
Notation "'WHILE' b 'DO' c 'END'" :=
  (CWhile b c) (at level 80, right associativity).
Notation "'IFB' e1 'THEN' e2 'ELSE' e3 'FI'" :=
  (CIf e1 e2 e3) (at level 80, right associativity).
Notation "'HAVOC' l" := (CHavoc l) (at level 60).

Reserved Notation "c1 '/' st '||' st'" (at level 40, st at level 39).

Inductive ceval : com -> state -> state -> Prop :=
  | E_Skip : forall st : state,
    SKIP / st || st
  | E_Ass : forall (st : state) (a1 : aexp) (n : nat) (X : id),
    aeval st a1 = n ->
    (X ::= a1) / st || update st X n
  | E_Seq : forall (c1 c2 : com) (st st' st'' : state),
    c1 / st || st' ->
    c2 / st' || st'' ->
    (c1 ;; c2) / st || st''
  | E_IfTrue : forall (st st' : state) (b1 : bexp) (c1 c2 : com),
    beval st b1 = true ->
    c1 / st || st' ->
    (IFB b1 THEN c1 ELSE c2 FI) / st || st'
  | E_IfFalse : forall (st st' : state) (b1 : bexp) (c1 c2 : com),
    beval st b1 = false ->
    c2 / st || st' ->
    (IFB b1 THEN c1 ELSE c2 FI) / st || st'
  | E_WhileEnd : forall (b1 : bexp) (st : state) (c1 : com),
    beval st b1 = false ->
    (WHILE b1 DO c1 END) / st || st
  | E_WhileLoop : forall (st st' st'' : state) (b1 : bexp) (c1 : com),
    beval st b1 = true ->
    c1 / st || st' ->
    (WHILE b1 DO c1 END) / st' || st'' ->
    (WHILE b1 DO c1 END) / st || st''
  | E_Havoc : forall (st : state) (n : nat) (x : id),
    (HAVOC x) / st || update st x n
  where "c1 '/' st '||' st'" := (ceval c1 st st').

Definition cequiv (c1 c2 : com) : Prop := forall st st' : state,
  c1 / st || st' <-> c2 / st || st'.

Definition clequiv (c1 c2 : com) : Prop := forall st st' : state,
  c1 / st || st' -> c2 / st || st'.

(** ** Sanity check examples *)

Example havoc_example1 : (HAVOC X) / empty_state || update empty_state X 0.
Proof.
  apply E_Havoc. Qed.

Example havoc_example2 :
  (SKIP;; HAVOC Z) / empty_state || update empty_state Z 42.
Proof.
  eapply E_Seq.
  apply E_Skip.
  apply E_Havoc. Qed.

(** ** Swap exercise *)

Definition pXY :=
  HAVOC X;; HAVOC Y.

Definition pYX :=
  HAVOC Y;; HAVOC X.

Lemma havoc_swap : forall (X Y : id),
  clequiv (HAVOC X;; HAVOC Y) (HAVOC Y;; HAVOC X).
Proof.
  intros X Y.
  intros st st'.
  intro H.
  destruct (eq_id_dec X Y).

  (* X = Y *)
  subst.
  assumption.

  (* X <> Y *)
  inversion H. subst.
  inversion H2. subst.
  inversion H5. subst.
  eapply E_Seq.
  apply E_Havoc.
  assert (swap : update (update st X n0) Y n1
               = update (update st Y n1) X n0).
    apply functional_extensionality.
    intro x.
    apply update_permute.
    assumption.
  rewrite -> swap.
  apply E_Havoc. Qed.

Theorem pXY_cequiv_pYX :
  cequiv pXY pYX \/ ~cequiv pXY pYX.
Proof.
  apply or_introl.
  unfold cequiv.
  intros st st'.
  split;
  apply havoc_swap. Qed.

(** ** Copy exercise **)

Definition ptwice :=
  HAVOC X;; HAVOC Y.

Definition pcopy :=
  HAVOC X;; Y ::= AId X.

Lemma pcopy_copy : forall (st st' : state),
  pcopy / st || st' -> st' X = st' Y.
Proof.
  unfold pcopy.
  intros st st'.
  intro H.
  inversion H. subst.
  inversion H2. subst.
  inversion H5. subst.
  reflexivity. Qed.

Theorem ptwice_cequiv_pcopy :
  cequiv ptwice pcopy \/ ~cequiv ptwice pcopy.
Proof.
  apply or_intror.
  intro Contra.
  remember (update (update empty_state X 0) Y 1) as st.

  assert (H1 : ptwice / empty_state || st).
    unfold ptwice.
    eapply E_Seq.
    apply E_Havoc.
    subst.
    apply E_Havoc.

  assert (H2 : pcopy / empty_state || st).
    apply Contra.
    assumption.

  assert (XeqY : st X = st Y).
    eapply pcopy_copy.
    apply H2.
  assert (Xeq0 : st X = 0).
    subst.
    reflexivity.
  assert (Yeq1 : st Y = 1).
    subst.
    reflexivity.
  rewrite -> Xeq0 in XeqY.
  rewrite -> Yeq1 in XeqY.
  inversion XeqY. Qed.

(** ** [p1] and [p2] equivalence *)

Definition p1 : com :=
  WHILE (BNot (BEq (AId X) (ANum 0))) DO
    HAVOC Y;;
    X ::= APlus (AId X) (ANum 1)
  END.

Definition p2 : com :=
  WHILE (BNot (BEq (AId X) (ANum 0))) DO
    SKIP
  END.

(** Following lemma makes next lemmas trivial to prove. Essentially, this is
    generalized version of [WHILE_true_nonterm] for regular Imp. It says that
    if we have some sort of loop invariant concluding that loop condition
    evaluates to true then the program does not terminate. *)

Lemma WHILE_true_nonterm : forall (P : state -> Prop) (b : bexp) (c : com),
  (forall (st : state), P st -> beval st b = true) ->
  (forall (st st' : state), P st -> c / st || st' -> P st') ->
  (forall (st st' : state), P st -> ~((WHILE b DO c END) / st || st')).
Proof.
  intro P.
  intros b c.
  intros Hb Hc.
  intros st st'.
  intro HP.
  intro Contra.
  remember (WHILE b DO c END) as cw.
  induction Contra;

  (* Trivial, contradiction cases. *)
  try (inversion Heqcw);
  subst.

  (* E_WhileEnd *)
  rewrite -> Hb in H.
  inversion H.
  assumption.

  (* E_WhileLoop *)
  apply IHContra2.
  eapply Hc.
  apply HP.
  apply Contra1.
  reflexivity. Qed.

Lemma p1p2_cond_eval : forall (st : state),
  st X <> 0 ->
  beval st (BNot (BEq (AId X) (ANum 0))) = true.
Proof.
  intro st.
  intro H.
  simpl.
  apply negb_true_iff.
  apply beq_nat_false_iff.
  assumption. Qed.

Lemma p1_may_diverge : forall st st', st X <> 0 ->
  ~ p1 / st || st'.
Proof.
  intros st st'.
  intro HX.
  apply WHILE_true_nonterm with (P := fun st => st X <> 0).

  apply p1p2_cond_eval.

  intros st0 st'0.
  intros HP Hc.
  inversion Hc. subst.
  inversion H1. subst.
  inversion H4. subst.
  unfold update.
  simpl.
  apply not_eq_sym.
  rewrite <- plus_comm.
  apply O_S.

  assumption. Qed.

Lemma p2_may_diverge : forall st st', st X <> 0 ->
  ~ p2 / st || st'.
Proof.
  intros st st'.
  intro HX.
  apply WHILE_true_nonterm with (P := fun st => st X <> 0).

  apply p1p2_cond_eval.

  intros st0 st'0.
  intros HP Hc.
  inversion Hc. subst.
  apply HP.

  assumption. Qed.

Theorem p1_p2_equiv : cequiv p1 p2.
Proof.
  unfold cequiv.
  intros st st'.
  destruct (eq_nat_dec (st X) 0).

  (* st X = 0 *)
  split;
  intro H;
  inversion H; subst;
  try (apply E_WhileEnd;
       simpl;
       rewrite -> e;
       reflexivity);
  try (simpl in H2;
       rewrite -> e in H2;
       simpl in H2;
       inversion H2).

  (* st X <> 0 *)
  split;
  intro H;
  try (apply p1_may_diverge in H);
  try (apply p2_may_diverge in H);
  try (contradiction H);
  assumption. Qed.

(** ** [p3] and [p4] inequivalence *)

Definition p3 : com :=
  Z ::= ANum 1;;
  WHILE (BNot (BEq (AId X) (ANum 0))) DO
    HAVOC X;;
    HAVOC Z
  END.

Definition p4 : com :=
  X ::= (ANum 0);;
  Z ::= (ANum 1).

Definition st_X1 : state := update empty_state X 1.
Definition st_Z0_X0 : state := update (update st_X1 Z 0) X 0.

Lemma p3_X0_Z0 :
  p3 / st_X1 || st_Z0_X0.
Proof.
  unfold p3.
  eapply E_Seq.
  apply E_Ass.
  reflexivity.
  apply E_WhileLoop with (st' := update (update (update st_X1 Z 1) X 0) Z 0).
  reflexivity.
  apply E_Seq with (st' := update (update st_X1 Z 1) X 0).
  apply E_Havoc.
  apply E_Havoc.
  assert (eval : update (update (update st_X1 Z 1) X 0) Z 0
               = update (update st_X1 Z 0) X 0).
    unfold update.
    apply functional_extensionality.
    intro V.
    destruct (eq_id_dec Z V);
    destruct (eq_id_dec X V);
    reflexivity.
  rewrite -> eval.
  apply E_WhileEnd.
  reflexivity. Qed.

Theorem p3_p4_inequiv : ~ cequiv p3 p4.
Proof.
  unfold cequiv.
  intro Contra.
  assert (H : p3 / st_X1 || st_Z0_X0).
    apply p3_X0_Z0.

  apply Contra in H.
  unfold p4 in H.
  inversion H. subst.
  inversion H2. subst.
  inversion H5. subst.
  simpl in H6.

  assert (H0 : st_Z0_X0 Z = 0).
    reflexivity.
  assert (H1 : st_Z0_X0 Z = 1).
    rewrite <- H6.
    reflexivity.
  rewrite -> H0 in H1.
  inversion H1. Qed.

(** ** [p5] and [p6] equivalence *)

Definition p5 : com :=
  WHILE (BNot (BEq (AId X) (ANum 1))) DO
    HAVOC X
  END.

Definition p6 : com :=
  X ::= ANum 1.

Lemma p5_p6_lequiv : clequiv p5 p6.
Proof.
  intros st st'.
  intro H.
  remember p5 as p5c.
  unfold p6.
  induction H;

  (* Trivial, contradiction cases. *)
  try (inversion Heqp5c);
  subst.

  (* E_WhileEnd *)
  simpl in H.
  rewrite -> negb_false_iff in H.
  apply beq_nat_true_iff in H.

  assert (Hst : update st X 1 = st).
    apply functional_extensionality.
    intro V.
    apply update_same.
    assumption.
  rewrite <- Hst at 2.
  apply E_Ass.
  reflexivity.

  (* E_WhileLoop *)
  assert (Hass : p6 / st' || st'').
    apply IHceval2.
    assumption.
  clear IHceval1 IHceval2 Heqp5c H H1.

  inversion H0. subst.
  inversion Hass. subst.
  assert (Hsimpl : update (update st X n) X (aeval (update st X n) (ANum 1))
                 = update st X 1).
    simpl.
    apply functional_extensionality.
    intro V.
    apply update_shadow.
  rewrite -> Hsimpl.

  apply E_Ass.
  reflexivity. Qed.

Lemma p6_p5_lequiv : clequiv p6 p5.
Proof.
  unfold p6.
  unfold p5.
  intros st st'.
  intro H.
  inversion H; subst.
  destruct (eq_nat_dec (st X) 1).

  (* X = 1 *)
  assert (Hsimpl : update st X (aeval st (ANum 1)) = st).
    apply functional_extensionality.
    intro V.
    apply update_same.
    rewrite -> e.
    reflexivity.
  rewrite -> Hsimpl.
  apply E_WhileEnd.
  simpl.
  rewrite -> e.
  reflexivity.

  (* X <> 1 *)
  eapply E_WhileLoop.
  simpl.
  rewrite -> negb_true_iff.
  apply beq_nat_false_iff.
  assumption.
  unfold beq_nat.
  apply E_Havoc.
  apply E_WhileEnd.
  reflexivity. Qed.

Theorem p5_p6_equiv : cequiv p5 p6.
Proof.
  split.
  apply p5_p6_lequiv.
  apply p6_p5_lequiv. Qed.

End Himp.


(* ######################################################################### *)
(** * Problem 4 (10 p.) *)
(** Solve the following exercises:
        - <<hoare_repeat>> ([Hoare]). *)

Module RepeatExercise.

(** ** Definitions *)
(** And for the third time some of the definitions were copied from [Hoare]
    module because of the namespacing issues. *)

Inductive com : Type :=
  | CSkip : com
  | CAsgn : id -> aexp -> com
  | CSeq : com -> com -> com
  | CIf : bexp -> com -> com -> com
  | CWhile : bexp -> com -> com
  | CRepeat : com -> bexp -> com.

Notation "'SKIP'" := 
  CSkip.
Notation "c1 ;; c2" := 
  (CSeq c1 c2) (at level 80, right associativity).
Notation "X '::=' a" := 
  (CAsgn X a) (at level 60).
Notation "'WHILE' b 'DO' c 'END'" := 
  (CWhile b c) (at level 80, right associativity).
Notation "'IFB' e1 'THEN' e2 'ELSE' e3 'FI'" := 
  (CIf e1 e2 e3) (at level 80, right associativity).
Notation "'REPEAT' e1 'UNTIL' b2 'END'" := 
  (CRepeat e1 b2) (at level 80, right associativity).

Reserved Notation "c1 '/' st '||' st'"
                  (at level 40, st at level 39).

Inductive ceval : state -> com -> state -> Prop :=
  | E_Skip : forall st,
    SKIP / st || st
  | E_Ass  : forall st a n X,
    aeval st a = n ->
    (X ::= a) / st || (update st X n)
  | E_Seq : forall c1 c2 st st' st'',
    c1 / st || st' ->
    c2 / st' || st'' ->
    (c1 ;; c2) / st || st''
  | E_IfTrue : forall st st' b1 c1 c2,
    beval st b1 = true ->
    c1 / st || st' ->
    (IFB b1 THEN c1 ELSE c2 FI) / st || st'
  | E_IfFalse : forall st st' b1 c1 c2,
    beval st b1 = false ->
    c2 / st || st' ->
    (IFB b1 THEN c1 ELSE c2 FI) / st || st'
  | E_WhileEnd : forall b st c,
    beval st b = false ->
    (WHILE b DO c END) / st || st
  | E_WhileLoop : forall st st' st'' b c,
    beval st b = true ->
    c / st || st' ->
    WHILE b DO c END / st' || st'' ->
    WHILE b DO c END / st || st''
  | E_RepeatEnd : forall b st st' c,
    c / st || st' ->
    beval st' b = true ->
    (REPEAT c UNTIL b END) / st || st'
  | E_RepeatLoop : forall b st st' st'' c,
    c / st || st' ->
    beval st' b = false ->
    (REPEAT c UNTIL b END) / st' || st'' ->
    (REPEAT c UNTIL b END) / st || st''
  where "c1 '/' st '||' st'" := (ceval st c1 st').

Definition hoare_triple (P : Assertion) (c : com) (Q : Assertion) : Prop :=
  forall st st', (c / st || st') -> P st -> Q st'.

Notation "{{ P }}  c  {{ Q }}" :=
  (hoare_triple P c Q) (at level 90, c at next level).

Definition clequiv (c1 c2 : com) := forall (st st' : state),
  c1 / st || st' -> c2 / st || st'.

(** ** Simple example *)

Definition ex1_repeat :=
  REPEAT
    X ::= ANum 1;;
    Y ::= APlus (AId Y) (ANum 1)
  UNTIL (BEq (AId X) (ANum 1)) END.

Theorem ex1_repeat_works :
  ex1_repeat / empty_state || update (update empty_state X 1) Y 1.
Proof.
  unfold ex1_repeat.
  apply E_RepeatEnd.
  eapply E_Seq.
  apply E_Ass.
  reflexivity.
  apply E_Ass.
  reflexivity.
  reflexivity. Qed.

(** ** Hoare rule for repeat loop *)
(** So, the rule for <<REPEAT>> loop is just composition of rules for sequence
    and <<WHILE>> loop - seems pretty general to me. The proof will follow
    from fact that <<REPEAT c UNTIL b END>> can be regarded as syntactic sugar
    for <<c;; WHILE BNot b DO c END>>. It surely can be proven directly, but
    I had the proofs for this equivalence anyway so it was easier like this. *)

(** First thing to prove is that these loops are in fact equivalent as long
    as they "roll" at least once. *)

Lemma repeat_while_loops_leq : forall (b: bexp) (c : com) (st st' : state),
  beval st b = false ->
  (REPEAT c UNTIL b END) / st || st' ->
  (WHILE (BNot b) DO c END) / st || st'.
Proof.
  intros b c.
  intros st st'.
  intros Hb H.

  remember (REPEAT c UNTIL b END) as rcom.
  induction H;

  (* Trivial, contradiction cases. *)
  try (inversion Heqrcom);
  subst.

  (* E_WhileEnd *)
  eapply E_WhileLoop.
  simpl.
  apply negb_true_iff.
  assumption.
  apply H.
  apply E_WhileEnd.
  simpl.
  apply negb_false_iff.
  assumption.

  (* E_WhileLoop *)
  eapply E_WhileLoop.
  simpl.
  apply negb_true_iff.
  assumption.
  apply H.
  apply IHceval2.
  assumption.
  reflexivity. Qed.

(** Having previous lemma it is relatively easy to show that <<REPEAT>> can be
    expressed as syntactic sugar. Since the complete equivalence is not needed
    only one implication suffices. *)

Lemma repeat_while_leq : forall (b : bexp) (c : com),
  clequiv (REPEAT c UNTIL b END) (c ;; WHILE (BNot b) DO c END).
Proof.
  intros b c.
  intros st st'.
  intro H.

  inversion H; subst.

  (* E_RepeatEnd *)
  eapply E_Seq.
  apply H3.
  apply E_WhileEnd.
  simpl.
  apply negb_false_iff.
  assumption.

  (*E_RepeatLoop *)
  eapply E_Seq.
  apply  H2.
  apply repeat_while_loops_leq.
  assumption.
  assumption. Qed.

(** One last thing before rule and final proof: because we are working with
    extended language rules proven so far are not applicable. Therefore,
    proof for <<WHILE>> loops rule has to be repeated. To be closer to the
    <<REPEAT>> loop, loop condition is negated. *)

Lemma hoare_not_while : forall (P : Assertion) (b : bexp) (c : com),
  {{ fun st => P st /\ ~(bassn b st) }} c {{ P }} ->
  {{ P }} WHILE BNot b DO c END {{ fun st => P st /\ bassn b st }}.
Proof.
  intro P.
  intros b c.
  intro Hloop.

  intros st st'.
  intros H Hpred.

  remember (WHILE BNot b DO c END) as wcom.
  induction H;

  (* Trivial, contradiction cases. *)
  try (inversion Heqwcom);
  subst;
  simpl in H.

  (* E_WhileEnd *)
  apply negb_false_iff in H.
  split.
  assumption.
  apply bexp_eval_true.
  assumption.

  (* E_WhileLoop *)
  apply negb_true_iff in H.
  apply IHceval2.
  reflexivity.
  apply (Hloop st st').
  assumption.
  split.
  assumption.
  apply bexp_eval_false.
  assumption. Qed.

Lemma hoare_repeat : forall (P Q : Assertion) (b : bexp) (c : com),
  {{ P }} c {{ Q }} ->
  {{ fun st => Q st /\ ~(bassn b st) }} c {{ Q }} ->
  {{ P }} REPEAT c UNTIL b END {{ fun st => Q st /\ bassn b st }}.
Proof.
  intros P Q.
  intros b c.
  intros Hstep Hloop.

  intros st st'.
  intros H Hpred.

  apply repeat_while_leq in H.
  inversion H. subst.

  (* Coq was not able to apply hoare_not_while without this workaround. *)
  assert (forall (P : Assertion) (b : bexp) (c : com),
          {{ fun st => P st /\ ~(bassn b st) }} c {{ P }} ->
          {{ P }} WHILE BNot b DO c END {{ fun st => P st /\ bassn b st }}).
    apply hoare_not_while.

  eapply H0.
  apply Hloop.
  apply H5.

  apply (Hstep st st'0).
  apply H3.
  apply Hpred. Qed.

(** Only thing left is to prove following Hoare triple:

    <<
    {{ X > 0 }}
    REPEAT
      Y ::= X;;
      X ::= X - 1
    UNTIL X = 0 END
    {{ X = 0 /\ Y > 0 }}
    >>

    Using repeat rule with <<P = (X > 0)>> and <<Q = (Y > 0)>> it suffices
    to prove two propositions:

    <<
    {{ X > 0 }}
    Y ::= X;;
    X ::= X - 1
    {{ Y > 0 }}
    >>

    <<
    {{ Y > 0 /\ X <> 0 }}
    Y ::= X;;
    X ::= X - 1
    {{ Y > 0 }}
    >>

    Both of them are trivial.
*)

End RepeatExercise.

(* ######################################################################### *)
(** * Problem 5 (5 p.) *)
(** Solve the following exercises:
        - <<add_slowly_decoration>> (Hoare2). *)

Example add_slowly_dec (n m : nat) := (
  {{ fun st => st Z = n /\ st X = m }} ->>
  {{ fun st => st Z + st X = n + m }}
  WHILE BNot (BEq (AId X) (ANum 0)) DO
     {{ fun st => st Z + st X = n + m /\ st X <> 0 }} ->>
     {{ fun st => (st Z + 1) + (st X - 1) = n + m }} 
     Z ::= APlus (AId Z) (ANum 1)
     {{ fun st => st Z + (st X - 1) = n + m }};;
     X ::= AMinus (AId X) (ANum 1)
     {{ fun st => st Z + st X = n + m }}
  END
  {{ fun st => st Z + st X = n + m /\ st X = 0 }} ->>
  {{ fun st => st Z = n + m }}
) % dcom.

Theorem add_slowly_dec_correct : forall (n m : nat),
  dec_correct (add_slowly_dec n m).
Proof.
  intros n m.
  verify. Qed.

(* ######################################################################### *)
(** * Problem 6 (5 p.) *)
(** Solve the following exercises:
        - <<parity_formal>> (Hoare2). *)

Lemma bassn_lt_2 : forall (st : state),
  bassn (BLe (ANum 2) (AId X)) st <-> 2 <= st X.
Proof.
  intros st.
  split;
  intro H.

  (* -> *)
  inversion H.
  destruct (st X).
  inversion H1.
  destruct n.
  inversion H1.
  apply le_n_S.
  apply le_n_S.
  apply le_0_n.

  (* <- *)
  unfold bassn.
  simpl.
  inversion H.
  reflexivity.
  inversion H1.
  reflexivity.
  reflexivity. Qed.

Theorem parity_correct : forall m,
  {{ fun st => st X = m }}
  WHILE BLe (ANum 2) (AId X) DO
    X ::= AMinus (AId X) (ANum 2)
  END
  {{ fun st => st X = parity m }}.
Proof.
  intro m.
  apply hoare_consequence_pre with
    (P' := fun st => parity (st X) = parity m).
  eapply hoare_consequence_post.
  apply hoare_while.
  eapply hoare_consequence_pre.
  apply hoare_asgn.

  unfold assert_implies.
  intro st.
  intro H.
  unfold assn_sub.
  simpl.
  rewrite -> update_eq.
  remember (st X) as x.
  inversion H.
  rewrite <- H0.
  apply parity_ge_2.
  subst.
  apply bassn_lt_2.
  assumption.

  intro st.
  intro H.
  inversion H.
  destruct (le_dec 2 (st X)).
  apply bassn_lt_2 in l.
  contradiction.
  rewrite <- H0.
  symmetry.
  apply parity_lt_2.
  assumption.

  unfold assert_implies.
  intro st.
  intro H.
  rewrite -> H.
  reflexivity. Qed.

(** * Problem 7 (5 p.) *)
(** Solve the following exercises:
        - <<factorial_dec>> (Hoare2). *)

Example factorial_dec (n : nat) : dcom := (
  {{ fun st => st X = n }}
  Z ::= AId X
  {{ fun st => st Z = n }};;
  Y ::= ANum 1
  {{ fun st => st Z = n /\ st Y = 1 }} ->>
  {{ fun st => real_fact n = st Y * real_fact (st Z) }};;
  WHILE (BNot (BEq (AId Z) (ANum 0))) DO
    {{ fun st => real_fact n = st Y * real_fact (st Z)
              /\ (st Z <> 0) }} ->>
    {{ fun st => real_fact n = st Y * st Z * real_fact (st Z - 1) }}
    Y ::= AMult (AId Y) (AId Z)
    {{ fun st => real_fact n = st Y * real_fact (st Z - 1) }};;
    Z ::= AMinus (AId Z) (ANum 1)
    {{ fun st => real_fact n = st Y * real_fact (st Z) }}
  END
  {{ fun st => real_fact n = st Y * real_fact (st Z)
            /\ st Z = 0 }} ->>
  {{ fun st => st Y = real_fact n }}
) % dcom.

Theorem factorial_dec_correct : forall (n : nat),
  dec_correct (factorial_dec n).
Proof.
  intro n.
  verify.

  destruct (st Z) as [| z'].
    (* st Z = 0 *)
    assert (0 = 0). reflexivity.
    contradiction.

    (* st Z <> 0 *)
    assert (real_fact (S z') = S z' * real_fact z'). reflexivity.
    rewrite -> H1 in H.
    rewrite -> mult_assoc in H.
    assert (S z' - 1 = z'). omega. (* reflexivity too dumb. *)
    rewrite -> H2.
    assumption.

  simpl in H.
  rewrite -> mult_1_r in H.
  symmetry.
  assumption. Qed.
