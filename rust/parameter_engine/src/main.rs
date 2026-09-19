#[derive(Debug,Clone,Copy)] struct Params{entry:f64,exit:f64,add_on:f64}
fn update(p:Params, win_rate:f64, spread:f64)->Params{ let mut x=p; if win_rate<0.50{x.entry+=0.03;x.add_on+=0.04;} else if win_rate>0.68{x.entry-=0.02;x.add_on-=0.02;} if spread>3.0{x.entry+=0.04;} x.entry=x.entry.clamp(0.50,0.90);x.add_on=x.add_on.clamp(0.65,0.95);x }
fn main(){let p=update(Params{entry:0.62,exit:0.20,add_on:0.78},0.61,1.8);println!("adaptive params: {:?}",p);}
