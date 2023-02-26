package org.xtext.paretoalgebra.calculator.compiler;

import java.io.File;
import java.nio.file.Files;
import java.util.List;
import java.util.Map.Entry;

import org.apache.commons.io.FilenameUtils;
import org.eclipse.emf.common.util.URI;
import org.eclipse.emf.ecore.resource.Resource;
import org.eclipse.emf.ecore.resource.ResourceSet;
import org.eclipse.xtext.generator.IGenerator2;
import org.eclipse.xtext.diagnostics.Severity;
import org.eclipse.xtext.generator.GeneratorContext;
import org.eclipse.xtext.generator.JavaIoFileSystemAccess;
import org.eclipse.xtext.util.CancelIndicator;
import org.eclipse.xtext.resource.XtextResource;
import org.eclipse.xtext.validation.CheckMode;
import org.eclipse.xtext.validation.IResourceValidator;
import org.eclipse.xtext.validation.Issue;
import org.xtext.paretoalgebra.calculator.CalculatorStandaloneSetup;

import com.google.inject.Injector;
import com.google.inject.Inject;
import com.google.inject.Provider;

public class ParetoCalculatorCompiler {

	@Inject
	private Provider<ResourceSet> resourceSetProvider;

	@Inject
	private IGenerator2 generator;

	@Inject
	private JavaIoFileSystemAccess fsa;

	private static void exitError(String msg) {
		System.out.println(msg);
		System.exit(-1);
	}

	private static void showUsage() {
		System.out.println("Usage:");
		System.out.println("pareto-calculator-compiler <filename.pareto> <outputdir>");
	}

	public static void main(String[] args) {

		if (args.length != 2) {
			showUsage();
			exitError("Please provide an input file as an argument and an output directory.");
		}

		String filename = args[0];
		String outputdir = args[1];

		String ext = FilenameUtils.getExtension(filename).toUpperCase();
		if ((! ext.equals("PARETO"))) {
			showUsage();
			exitError("The provided filename does not have a .pareto extension.");
			}

		File f = new File(filename);
		if( (! f.exists()) || f.isDirectory()) {
			showUsage();
			exitError("The provided filename does not exist.");
		}

		File d = new File(outputdir);
		if (!d.exists() || ! d.isDirectory()) {
			showUsage();
			exitError("The output directory does not exist.");
		}

		Injector injector = null;
		
		switch (ext)
		{
		case "PARETO":
			// do this only once per application
			injector = new CalculatorStandaloneSetup().createInjectorAndDoEMFRegistration();
			break;
		}
		
		if (injector != null) {
			ParetoCalculatorCompiler comp = injector.getInstance(ParetoCalculatorCompiler.class);
			comp.runGenerator(filename, outputdir);
		} else {
			exitError("Failed to create injector.");
		}
	}


	protected void runGenerator(String filename, String outputdir) {
		// obtain a resourceset
		ResourceSet resourceSet = resourceSetProvider.get();

		// load a resource by URI, in this case from the file system
		Resource resource = resourceSet.getResource(URI.createFileURI(filename), true);

		// Validation
		IResourceValidator validator = ((XtextResource)resource).getResourceServiceProvider().getResourceValidator();
		List<Issue> issues = validator.validate(resource, CheckMode.ALL, CancelIndicator.NullImpl);
		boolean errorEncountered = false;
		for (Issue issue : issues) {
		  System.out.println(issue.getMessage());
		  if (issue.getSeverity() == Severity.ERROR) {
			  errorEncountered = true;
		  }
		}

		if (errorEncountered ) {
			System.exit(-1);
		}

		// Code Generator
		fsa.setOutputPath(outputdir);
		generator.doGenerate(resource, fsa, new GeneratorContext());
	}
}

