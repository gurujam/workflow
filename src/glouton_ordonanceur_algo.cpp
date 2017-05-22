include "assembly.h"
include "machine.h"
include "reference_composant.h"

public class ordonanceur {
		hashmap reference_Interne_Port <pair<(instance, port)>, asset_port>;
		hashmap reference_component_started <instance, reference_Instance>
		hashmap hashmap_dataSend <pair<(instance, port)>, boolean>;

		Declare actual_declare;

		pile ([Declare]) stack_actual_declare;

	/* ------------------------------------------------------------------------ */
	/* 					FONCTION DETERMINISTE POUR LA MACHINE 					*/
	/* ------------------------------------------------------------------------ */
	reference_machine get_machine_with_ressources(){
		//TODO 
		//C'est ce qui permet de regarder l'ensemble des machines et de renvoyer une machine qui dispose des ressources demandé (ram, cpu ...)
	}
	/* ------------------------------------------------------------------------ */
	/* 					FONCTION D'INSTANCIATION DE COMPOSANT 					*/
	/* ------------------------------------------------------------------------ */
	reference_Instance instanciation_component(Instance i){
	    if (empty(reference_component_started.get(i))){
	      Ressources r = i.getNecessaryRessource(); //optionnal
	      reference_machine m = this.get_machine_with_ressources(r); //optionnal
	      reference_Instance instancied_component = m.initialize_component(i.getPath(), i.getname(), i.getSign(), i.getComponent());
	      reference_component_started.add(i, instancied_component);
	      return instancied_component;
	    }else{
	      return reference_component_started.get(i);
	    }
	}
	/* ------------------------------------------------------------------------ */
	/* 					FONCTION DE CONNEXION DES PORT DE SORTIE 				*/
	/* ------------------------------------------------------------------------ */
	void connect_OutPorts_for_Intance(reference_Instance rI, Instance i){
	    for (each port in i.get_outPorts()){
	        rI.connect(port.getName, reference_Interne_Port (i, port));
	    }
	}
	/* ------------------------------------------------------------------------ */
	/* 					CONNEXION ENTRE DEUX PORT DE COMPOSANT 					*/
	/* ------------------------------------------------------------------------ */
	void direct_connect (reference_Instance ref1, reference_Instance ref2, String portName1, String portName2){
		ref1.connect(portName1, ref2.get_reference_port(portName2));
	}
	/* ------------------------------------------------------------------------ */
	/* 			CONNEXION DIRECTE DE TOUT LES CONFIGPORT D'UN DECLARE			*/
	/* ------------------------------------------------------------------------ */
	void connect_all_direct (Declade d){
		for (each cp in d.getconfigPort()){
			reference_Instance ref1 = reference_component_started(cp.getInPortInstance())
			reference_Instance ref2 = reference_component_started(cp.getOutPortInstance())
			String portName1 = cp.getInPortName();
			String portName2 = cp.getOutPortName();
			direct_connect(ref1, ref2, portName1, portName2);
		}
	}
	/* ------------------------------------------------------------------------ */
	/* 				FONCTION D'ENVOIE DE DONNÉE VERS LE COMPOSANT 				*/
	/* ------------------------------------------------------------------------ */
	void sendDataToInstance(Data d, reference_Instance instancied_component, Port p){
	   	//if (!empty(hashmap_dataSend.get(instancied_component, p))){ 
	    //Decommenté pour ne pouvoir envoyé qu'une fois les données
	      instancied_component.sendDataForPort(data, port.getname());
	      hashmap_dataSend.add(instancied_component, p, true);
	    //}
	}
	/* ------------------------------------------------------------------------ */
	/* 		FONCTION POSITIONNEMENT DES DONNÉES ENTRANTE D'UN COMPOSANT 		*/
	/* ------------------------------------------------------------------------ */
	void set_input_for_instance(instance i){//SETALLINPUT
	    reference_component component_reference = reference_component_started.get(i);
	    if (component_reference == null){
	        component_reference = instanciation_component(i);
	    }

	    connect_OutPorts_for_Intance(component_reference, i);
	    boolean exist_in_configPort;
	    boolean exist_inPort = false;
	    for (each Port p in i){
	    	exist_inPort = true;
	    	exist_in_configPort = false;
	        for (each ConfigPort cp in actual_declare.getconfigPort()){
	            if (cp.in == p){
	            	exist_in_configPort = true;
	            	data = reference_Interne_Port(cp.out.getInstance(), cp.out.getPort()).getValue();
	                if (data != null) {
	                    sendDataToInstance(data, component_reference, p);
	                }else{
	                    component_reference.setvoid(p.getname());
	                }
	            }
	        }
	        if (!exist_in_configPort && p.isInputPort()){
	        	exist_inPort = true;
	        	component_reference.setvoid(p.getname());
	        }
	    }
	    if (!exist_inPort){
	    	component_reference.setvoid();
	    }
	}
	/* ------------------------------------------------------------------------ */
	/* 						AJOUT D'UN DECLARE EXTERNE							*/
	/* ------------------------------------------------------------------------ */
	void add_declare(Declare d){
		stack_actual_declare.push(d);
		Declare act = new Declare ();
		for (each d in stack_actual_declare){
			act.add(d);
		}
		actual_declare = act;

	}
	/* ------------------------------------------------------------------------ */
	/* 			ATTENTE DE RECEPTION DE TOUTE LES DONNÉES SORTANTES				*/
	/* ------------------------------------------------------------------------ */
		void waiting_end_of_instance(Instance i){
			boolean total_reception = false;
			boolean internal_reception = false;
			while (!total_reception){
				sleep(10);
				internal_reception = true;
				for (each pOut in i.get_outPorts()){
					if (reference_Interne_Port(pOut, i) == null){
						internal_reception = false;
					}
				}
				if (internal_reception){
					total_reception = true;
				}
			}
		}
	/* ------------------------------------------------------------------------ */
	/* 					SUPRRESSION D'UN DECLARE EXTERNE						*/
	/* ------------------------------------------------------------------------ */
	void delete_last_declare(){
		stack_actual_declare.pull();
		Declare act = new Declare ();
		for (each d in stack_actual_declare){
			act.add(d);
		}
		actual_declare = act;

	}
	/* ------------------------------------------------------------------------ */
	/* 						TRAITEMENT D'INSTRUCTION 							*/
	/* ------------------------------------------------------------------------ */
	void execute_instruction (Instruction i){
	    switch (i.get_instruction_type()){

		    case execTask : 
		    	reference_component_started((execTask)i.getInstance()).setvoid();
		    	waiting_end_of_instance((execTask)i.getInstance());
		    	reference_component_started((execTask)i.getInstance()).end();//maybe
		    	break;

		    case sequence :
		    	//Ajout des connexions qui doivent être disponible pour le bloc séquence
			    add_declare((sequence)i.getDeclare());
			    for (each instr in (sequence)i.getDeclare().getInstance()){
			    	instanciation_component(instr);
			    }
			    connect_all_direct((sequence)i.getDeclare());

			    for (each inst in (sequence)i.declare.getAllInstance()){
			        for (each pOut in inst.get_outPorts()){
			            reference_Interne_Port(pOut, inst) = new asset_port ();
			        }
			        connect_OutPorts_for_Intance(reference_component_started(inst), inst);
			    }

			    //Execution de l'ensemble des instructions
			    for (each instr in (sequence)i.getAllInstruction()){
			        execute_instruction (instr);
			    }
			    //On supprime les connexion qui ne doivent être valable que pour le bloc séquence
			    for (each instr in (sequence)i.declare.getAllInstruction()){
			        for (each pOut in instr.get_outPorts()){
			            reference_Interne_Port(pOut, instr).delete();
			        }
			        
			    }
			    delete_last_declare();
		    	break;
	    }
	}
	/* ------------------------------------------------------------------------ */
	/* 									MAIN 									*/
	/* ------------------------------------------------------------------------ */
	main (){
		assembly internal_assembly = load(assembly_to_use); //On determine l'assembly qu'on veux utiliser
		stack_actual_declare.push(internal_assembly.getDeclare());
		actual_declare = internal_assembly.getDeclare();

		for (each inst in internal_assembly.getDeclare().getAllInstances()){
			instanciation_component(instr);
			for (each pOut in instr.get_outPorts()){
				reference_Interne_Port(pOut, inst) = new asset_port();
			}
			connect_OutPorts_for_Intance(reference_component_started(inst), inst);
		}

		connect_all_direct(actual_declare);
		execute_instruction(internal_assembly.getInstruction());
	}



}

