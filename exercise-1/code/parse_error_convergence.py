def parse_err_convergence_line(line):
	step = analytic = float(line.split(" ")[0])
	analytic = float(line.split(" ")[5])
	vals = map(float,line.split(" ")[1:5])
	err = [abs(i - analytic) for i in vals]
	return [step, analytic, vals, err]

methods = ["euler" ,"symplectic_euler", "midpoint", "backwards_euler"]

a = open("displacement_results_no_damping.txt", "r").readlines()
dis_errors = []

# compute displacement errors
steps = []
for i in range(len(a)):
	# parse errors
	[step, analytic, vals, err] = parse_err_convergence_line(a[i])
	dis_errors.append(err)
	steps.append(step)

print "---------- displacement errors ------------\n"
for i in range(len(a)):
	out = "Step " + (str(steps[i])+"\t").expandtabs(8) + ") "
	cur_error = dis_errors[i]
	for m in range(len(methods)):
		out = out + (methods[m] +"\t").expandtabs(10) + ": " + (str(cur_error[m])+"\t").expandtabs(20)
	print out

# compute relative errors (e(i)/e(i+1) for each method)
rel_errors_avg_sum = [0 for i in range(len(a)-1)]
for i in range(1,len(a)-1):

	err1 = dis_errors[i-1]
	err2 = dis_errors[i]
	
	rel_errors = [ e1 / e2 for (e2, e1) in zip(err2, err1)]
	
	# sum up errors
	rel_errors_avg_sum = [err1 + err2 for (err1,err2) in zip(rel_errors, rel_errors_avg_sum)]

rel_errors_avg = [num/(len(a)-1) for num in rel_errors_avg_sum]
print "\n\n---------- Error Convergence Average ------------\n"
out = ""
for m in range(len(methods)):
	out = out + (methods[m] +"\t").expandtabs(10) + ": " + (str(rel_errors_avg[m])+"\t").expandtabs(20)
print out
