#[derive(Clone,Debug)]pub struct FeatureSnapshot{pub timestamp_ns:u64,pub pressure:f64,pub depth_imbalance:f64,pub spread:f64,pub event_strength:f64,pub volatility:f64}
#[derive(Clone,Debug)]pub struct ParameterState{pub regime_id:u32,pub generation:u64,pub valid:bool}
pub trait ParameterEngine{fn update(&mut self,s:&FeatureSnapshot)->ParameterState;}
pub struct RedactedParameterEngine{generation:u64}impl RedactedParameterEngine{pub fn new()->Self{Self{generation:0}}}impl Default for RedactedParameterEngine{fn default()->Self{Self::new()}}
impl ParameterEngine for RedactedParameterEngine{fn update(&mut self,s:&FeatureSnapshot)->ParameterState{self.generation+=1;ParameterState{regime_id:if s.volatility>0.0{1}else{0},generation:self.generation,valid:false}}}
