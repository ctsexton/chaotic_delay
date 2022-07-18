ChaoticDelay : UGen {
	*ar { |input, feedback, rate|
		^this.multiNew('audio', input, feedback, rate);
	}
	checkInputs {
		/* TODO */
		^this.checkValidInputs;
	}
}
