#include <assert.h>



void before() 
{
    printer = new Printer();
    document = new Document();
    job = new Job(document);
}

void after() 
{
}

void testStart() 
{
    // par défaut un printer n'est pas activé
    assertEquals(printer.start(), true);
    assertEquals(printer.start(), false);

    //on désactive le printer
    printer.stop();
}

void testStop() 
{
    //dans un premier temps on active le printer
    printer.start();

    printer.print(job);

    //une imprimante en cours d'impression ne peux pas être arrêté
    assertEquals(printer.stop(),false);

    printer.suspend();
    //Si une impression est suspendue, mais pas cancel, l'imprimante ne peut toujours pas être arrêtée
    assertEquals(printer.stop(),false);
    printer.resume();
    printer.printingCompleted();

    //on test le stop
    assertEquals(printer.stop(), true);
    //une fois innactif, on ne peut pas arrêter un printer non actif
    assertEquals(printer.stop(), false);

}

void testPrint() 
{
    //on ne peut pas demander à une imprimante inactive d'imprimer
    assertEquals(printer.print(job), false);

    printer.start();

    assertEquals(printer.print(job), true);
    //une imprimante en cours d'impression ne peut pas accepter une nouvelle impression
    assertEquals(printer.print(job), false);

    printer.printingCompleted();

    printer.stop();

}

void testPrintingCompleted() 
{
    //une imprimante inactive ne peux pas avoir complété une tâche
    assertEquals(printer.printingCompleted(), false);

    printer.start();

    //Une imprimante n'ayant pas de travail à réaliser ne peut pas avoir complétè une tâche.
    assertEquals(printer.printingCompleted(), false);

    printer.suspend();
    //une impression suspendue ne peux pas être complété
    assertEquals(printer.printingCompleted(), false);

    printer.resume();

    printer.print(job);
    assertEquals(printer.printingCompleted(), true);

    printer.stop();


}

void testSuspend() 
{
    //une imprimante inactive ne peux pas être suspendue
    assertEquals(printer.suspend(), false);

    printer.start();
    //une imprimante active, mais sans impression ne peux pas être suspendue
    assertEquals(printer.suspend(), false);

    printer.print(job);
    assertEquals(printer.suspend(), true);

    // une impression déjà suspendue ne peut pas être une nouvelle fois suspendue
    assertEquals(printer.suspend(), false);

    printer.printingCompleted();
    printer.stop();

}

void testResume() 
{
    // une imprimante inactive ne peux pas reprendre une impression
    assertEquals(printer.resume(), false);

    printer.start();
    //une imprimante n'ayant pas de travail a faire ne peux pas reprendre une impression
    assertEquals(printer.resume(), false);

    printer.print(job);
    //une imprimante qui est déjà en train d'imprimer ne peux pas reprendre son impression
    assertEquals(printer.resume(),false);

    printer.suspend();
    assertEquals(printer.resume(), true);

    //une fois l'impression reprise, on ne peux pas reprendre une seconde fois l'impression
    assertEquals(printer.resume(),false);

    printer.printingCompleted();
    printer.stop();

}

void testCancel() 
{
    //une imprimante inactive ne peux pas se voir annuler une impression
    assertEquals(printer.cancel(),false);

    //tant qu'une imprimante n'a pas une impression en suspession, il sera impossible d'annuler une impression
    printer.start();
    assertEquals(printer.cancel(),false);
    printer.print(job);
    assertEquals(printer.cancel(),false);

    printer.suspend();
    assertEquals(printer.cancel(),true);

    //une fois une impression annulée, nous ne pouvons pas la réanuler
    assertEquals(printer.cancel(),false);

    printer.stop();

}

int main(int argc, const char **argv)
{
}
