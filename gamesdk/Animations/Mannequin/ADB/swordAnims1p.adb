<AnimDB FragDef="Animations/Mannequin/ADB/PlayerActions.xml" TagDef="Animations/Mannequin/ADB/PlayerTags.xml">
 <FragmentList>
  <melee>
   <Fragment BlendOutDuration="0.2" Tags="Sword">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_hit_downVerticall_1Hsword_add_1p_01"/>
    </AnimLayer>
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="Sword">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_hit_frontPricking_1Hsword_add_1p_01"/>
    </AnimLayer>
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="Sword">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_hit_leftHorizontal_1Hsword_add_1p_01"/>
    </AnimLayer>
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="Sword">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_hit_rightDiagonal_1Hsword_add_1p_01"/>
    </AnimLayer>
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="Sword">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_hit_rightHorizontal_1Hsword_add_1p_01"/>
    </AnimLayer>
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="Sword">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_hit_upVerticall_1Hsword_add_1p_01"/>
    </AnimLayer>
   </Fragment>
  </melee>
  <select>
   <Fragment BlendOutDuration="0.2" Tags="Sword">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_select_1Hsword_add_1p_01"/>
    </AnimLayer>
   </Fragment>
  </select>
  <deselect>
   <Fragment BlendOutDuration="0.2" Tags="Sword">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_deselect_1Hsword_add_1p_01"/>
    </AnimLayer>
   </Fragment>
  </deselect>
  <idle_break>
   <Fragment BlendOutDuration="0.2" Tags="Sword">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_idleBreak_1Hsword_add_1p_01"/>
    </AnimLayer>
   </Fragment>
  </idle_break>
  <idlePose>
   <Fragment BlendOutDuration="0.2" Tags="Sword+defence">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.44999999"/>
     <Animation name="stand_defence_sword_add_1p_01" flags="Loop"/>
    </AnimLayer>
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="Sword">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_idlePose_1Hsword_1p_01" flags="Loop"/>
    </AnimLayer>
   </Fragment>
  </idlePose>
  <FPIdle>
   <Fragment BlendOutDuration="0.2" Tags="Sword">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_idle_sword_add_1p_01" flags="Loop"/>
    </AnimLayer>
   </Fragment>
  </FPIdle>
  <FPSway>
   <Fragment BlendOutDuration="0.2" Tags="Sword">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_look_dwn_1Hsword_add_1p_01" flags="Loop"/>
    </AnimLayer>
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_look_up_1Hsword_add_1p_01" flags="Loop"/>
    </AnimLayer>
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_look_rgt_1Hsword_add_1p_01" flags="Loop"/>
    </AnimLayer>
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_look_lft_1Hsword_add_1p_01" flags="Loop"/>
    </AnimLayer>
   </Fragment>
  </FPSway>
  <FPMovement>
   <Fragment BlendOutDuration="0.2" Tags="Sword">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_idle_sword_add_1p_01" flags="Loop" speed="3"/>
    </AnimLayer>
   </Fragment>
  </FPMovement>
  <bump>
   <Fragment BlendOutDuration="0.2" Tags="Sword">
    <ProcLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.5"/>
     <Procedural type="WeaponBump">
      <ProceduralParams>
       <Time value="0.5"/>
       <Shift value="0.89999998"/>
       <Rotation value="3.7"/>
      </ProceduralParams>
     </Procedural>
    </ProcLayer>
   </Fragment>
  </bump>
  <release>
   <Fragment BlendOutDuration="0.2" Tags="Sword+up+vertical">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_release_upverticall_1hsword_1p_01"/>
    </AnimLayer>
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="Sword+down+vertical">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_release_downverticall_1hsword_1p_01"/>
    </AnimLayer>
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="Sword+leftSide+diagonal">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_release_leftdiagonal_1hsword_1p_01"/>
    </AnimLayer>
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="Sword+rightSide+diagonal">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_release_rightdiagonal_1hsword_1p_01"/>
    </AnimLayer>
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="Sword+rightSide+horizontal">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_release_righthorizontal_1hsword_1p_01"/>
    </AnimLayer>
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="Sword+leftSide+horizontal">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_release_lefthorizontal_1hsword_1p_01"/>
    </AnimLayer>
   </Fragment>
  </release>
  <swing>
   <Fragment BlendOutDuration="0.2" Tags="Sword+leftSide+diagonal">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_swing_leftDiagonal_1Hsword_1p_01"/>
     <Blend ExitTime="0.2" StartTime="0" Duration="1.4901161e-008"/>
     <Animation name="stand_hold_leftdiagonal_1hsword_1p_01" flags="Loop"/>
    </AnimLayer>
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="Sword+leftSide+horizontal">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_swing_leftHorizontal_1Hsword_1p_01"/>
     <Blend ExitTime="0.26666668" StartTime="0" Duration="0"/>
     <Animation name="stand_hold_lefthorizontal_1hsword_1p_01" flags="Loop"/>
    </AnimLayer>
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="Sword+rightSide+horizontal">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_swing_rightHorizontal_1Hsword_1p_01"/>
     <Blend ExitTime="0.23333335" StartTime="0" Duration="0"/>
     <Animation name="stand_hold_rightHorizontal_1Hsword_add_1p_01" flags="Loop"/>
    </AnimLayer>
    <AnimLayer />
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="Sword+rightSide+diagonal">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_swing_rightDiagonal_1Hsword_1p_01"/>
     <Blend ExitTime="-1" StartTime="0" Duration="0"/>
     <Animation name="stand_hold_rightDiagonal_1Hsword_add_1p_01" flags="Loop"/>
    </AnimLayer>
    <AnimLayer />
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="Sword+down+vertical">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_swing_downVerticall_1Hsword_1p_01"/>
     <Blend ExitTime="-1" StartTime="0" Duration="0.010833383"/>
     <Animation name="stand_hold_downVerticall_1Hsword_add_1p_01" flags="Loop"/>
    </AnimLayer>
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="Sword+up+vertical">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_swing_upVerticall_1Hsword_1p_01"/>
     <Blend ExitTime="-1" StartTime="0" Duration="0"/>
     <Animation name="stand_hold_upVerticall_1Hsword_add_1p_01" flags="Loop"/>
    </AnimLayer>
   </Fragment>
  </swing>
  <holding>
   <Fragment BlendOutDuration="0.2" Tags="Sword+rightSide+diagonal">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_hold_rightDiagonal_1Hsword_add_1p_01" flags="Loop"/>
    </AnimLayer>
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="Sword+rightSide+horizontal">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_hold_rightHorizontal_1Hsword_add_1p_01" flags="Loop"/>
    </AnimLayer>
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="Sword+up+vertical">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_hold_upVerticall_1Hsword_add_1p_01" flags="Loop"/>
    </AnimLayer>
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="Sword+leftSide+diagonal">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_hold_leftDiagonal_1Hsword_add_1p_01" flags="Loop"/>
    </AnimLayer>
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="Sword+leftSide+horizontal">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_hold_leftHorizontal_1Hsword_add_1p_01" flags="Loop"/>
    </AnimLayer>
   </Fragment>
   <Fragment BlendOutDuration="0.2" Tags="Sword+down+vertical">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_hold_downVerticall_1Hsword_add_1p_01" flags="Loop"/>
    </AnimLayer>
   </Fragment>
  </holding>
  <dodge>
   <Fragment BlendOutDuration="0.2" Tags="Sword">
    <AnimLayer>
     <Blend ExitTime="0" StartTime="0" Duration="0.2"/>
     <Animation name="stand_dodge_sword_add_1p_01"/>
    </AnimLayer>
   </Fragment>
  </dodge>
 </FragmentList>
</AnimDB>
