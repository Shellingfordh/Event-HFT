use parameter_engine::*;
#[test]fn interface(){let mut e=RedactedParameterEngine::new();let s=e.update(&FeatureSnapshot{timestamp_ns:1,pressure:.2,depth_imbalance:.1,spread:1.,event_strength:.5,volatility:.2});assert_eq!(s.generation,1);}
